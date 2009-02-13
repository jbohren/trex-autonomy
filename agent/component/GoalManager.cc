#include "PlanDatabase.hh"
#include "Token.hh"
#include "TokenVariable.hh"
#include "TemporalAdvisor.hh"
#include "DbCore.hh"
#include "Constraints.hh"
#include "Timeline.hh"
#include "Agent.hh"
#include "GoalManager.hh"


#include <math.h>

namespace TREX{

  bool isPositionDependentGoal(const EntityId& entity){
    checkError(TokenId::convertable(entity), "Invalid configuration for " << entity->toString());

    TokenId token(entity);
    if(!token->getState()->baseDomain().isMember(Token::REJECTED))
      return false;

    if(token->getVariable(GoalManager::X()).isNoId() || token->getVariable(GoalManager::Y()).isNoId())
      return false;

    if(!token->getVariable(GoalManager::X())->lastDomain().isSingleton() 
       || !token->getVariable(GoalManager::Y())->lastDomain().isSingleton())
      return false;

    return true;
  }

  GoalsOnlyFilter::GoalsOnlyFilter(const TiXmlElement& configData): FlawFilter(configData, true) {}

  bool GoalsOnlyFilter::test(const EntityId& entity){
    bool result = !isPositionDependentGoal(entity);
    debugMsg("GoalsOnlyFilter:test", (result ? "Excluding " : "Allowing ") << entity->toString());
    return result;
  }

  NoGoalsFilter::NoGoalsFilter(const TiXmlElement& configData): FlawFilter(configData, true) {}

  bool NoGoalsFilter::test(const EntityId& entity){
    bool result = isPositionDependentGoal(entity);
    debugMsg("NoGoalsFilter:test", (result ? "Excluding " : "Allowing ") << entity->toString());
    return result;
  }

  /**
   * @brief Constructor will read xml configuration data as needed. Can extend to include a map input source for example.
   * @todo Include a map input file as a configuration argument
   */
  GoalManager::GoalManager(const TiXmlElement& configData)
    : OpenConditionManager(configData),
      m_maxIterations(1000),
      m_plateau(5),
      m_positionSourceCfg(""),
      m_state(STATE_DONE) {

    // Set the robot's initial position to be the origin.
    m_position.x = 0;
    m_position.y = 0;

    // Read configuration parameters to override defaults

    debugMsg("GoalManager:starting", "Starting Up");

    // MAX ITERATIONS
    const char * maxI = configData.Attribute(CFG_MAX_ITERATIONS().c_str());
    if(maxI != NULL)
      m_maxIterations = atoi(maxI);

    // PLATEAU
    const char * plateau = configData.Attribute(CFG_PLATEAU().c_str());
    if(plateau != NULL)
      m_plateau = atoi(plateau);

    // POSITION SOURCE
    const char * positionSrc = configData.Attribute(CFG_POSITION_SOURCE().c_str());
    if(positionSrc != NULL)
      m_positionSourceCfg = LabelStr(positionSrc);
  }

  void GoalManager::step() {
    if (noMoreFlaws()) {
      return;
    }  

    if(m_state == STATE_REQUIRE_PLANNING){
      generateInitialSolution();
      
      m_iteration = m_watchDog = 0;
    }
    
    if (m_currentSolution.empty() && m_ommissions.empty()) {
      debugMsg("GoalManager", "No goals, quitting.");
      setState(STATE_DONE);
      return;
    }

    setState(STATE_PLANNING);
    if(m_iteration < m_maxIterations && m_watchDog < m_plateau) {    
      // Update counters to handle termination
      m_iteration++;
      m_watchDog++;
      
      // Get best neighbor
      TokenId delta;
      GoalManager::SOLUTION candidate;
      selectNeighbor(candidate, delta);
      
      // In the event that the candidate is the same solution, we have hit the end of exploration unless
      // we escape somehow. The algorithm does not include such random walks to explore beyond the immediate
      // neighborhood
      if(candidate == m_currentSolution) {
	setState(STATE_DONE);
      } else {
	// Promote if not worse. Allos for some exploration
	int result = compare(candidate, m_currentSolution);
	if(result >= 0) {
	  // If a token was removed, insert into ommitted token list, and opposite if appended
	  if(m_currentSolution.size() > candidate.size())
	    m_ommissions.insert(delta);
	  else if(m_currentSolution.size() < candidate.size())
	    m_ommissions.erase(delta);
	  
	  // Update current solution Values
	  m_currentSolution = candidate;
	  
	  // Only pet the watchdog if we have improved the score. This allows us to terminate when
	  // we have plateaued
	  if(result == 1)
	    m_watchDog = 0;
	  
	  debugMsg("GoalManager:search", "Switching to new solution: " << toString(m_currentSolution));
	} else {
	  setState(STATE_DONE);
	}
      }
    } else {
      setState(STATE_DONE);
    }
    if ( m_state == STATE_DONE) {
      debugMsg("GoalManager:search", "Returning: " << toString(m_currentSolution));
    }
  }


  bool GoalManager::noMoreFlaws() {
    return m_state == STATE_DONE;
  }

  bool GoalManager::isNextToken(TokenId token) {
    if (!noMoreFlaws()) {
      return false;
    }
    TokenId nextGoal = TokenId::noId();
    for(SOLUTION::const_iterator it = m_currentSolution.begin(); it != m_currentSolution.end(); ++it){
      TokenId t = *it;
      checkError(t.isValid(), "A token that has been deleted is in the solution.");
      if (t->isInactive()){
	nextGoal = t;
	break;
      }
    }
    return (nextGoal == token);
  }

  /**
   * @brief When a new flaw is added we will re-evaluate all the options. This is accomplished
   * by incrementing a cycle count which makes existing solution stale.
   */
  void GoalManager::addFlaw(const TokenId& token){
    OpenConditionManager::addFlaw(token);


    if(token->getState()->baseDomain().isMember(Token::REJECTED)){
      debugMsg("GoalManager:addFlaw", "Adding flaw: " << token->toString());
      setState(STATE_REQUIRE_PLANNING);
    }
  }


  void GoalManager::removeFlaw(const TokenId& token){
    
    OpenConditionManager::removeFlaw(token);

    debugMsg("GoalManager", "Removing flaw: " << token->toString());

    if(token->isRejected()) {
      setState(STATE_REQUIRE_PLANNING);
    }
    else {
      SOLUTION::iterator it = m_currentSolution.begin();
      while(it != m_currentSolution.end()){
	TokenId t = *it;
	if(t == token){
	  m_currentSolution.erase(it);
	  break;
	}
	++it;
      }
    }

    // Clear from ommittedTokens
    m_ommissions.erase(token);
  }

  void GoalManager::handleInitialize(){
    static const LabelStr sl_nullLabel("");
    OpenConditionManager::handleInitialize();

    if(m_positionSourceCfg != sl_nullLabel){
      debugMsg("GoalManager:handleInitialize", "Looking to source position information from '" << m_positionSourceCfg.toString() << "'");
      m_positionSource = getPlanDatabase()->getObject(m_positionSourceCfg);
      checkError(m_positionSource.isValid(), 
		 "No position source for GoalManager. See Goal Manager Configuration in solver to set the source for getting position data.");
    }
  }

  /**
   * @brief Generates a simple seed solution
   */
  void GoalManager::generateInitialSolution(){
    debugMsg("GoalManager:generateInitialSolution", "Resetting solution");
    
    m_currentSolution.clear();
    m_ommissions.clear();

    // Set the initial conditions since the problem may have moved on
    setInitialConditions();

    // The empty solution is the default solution
    IteratorId it = OpenConditionManager::createIterator();
    while(!it->done()){
      TokenId goal = (TokenId) it->next();
      checkError(goal->master().isNoId(), goal->toString());
      debugMsg("GoalManager:generateInitialSolution", "Adding token to system: " << goal->toString());
      m_ommissions.insert(goal);
    }

    delete (FlawIterator*) it;
  }


  GoalManager::~GoalManager(){}


  /**
   * @brief For now we use euclidean distance. This is where map integration comes in.
   */
  double GoalManager::computeDistance(const Position& p1, const Position& p2){
    double result = sqrt(pow(p1.x-p2.x, 2) + pow(p1.y-p2.y, 2));
    debugMsg("GoalManager:computeDistance", "Distance between (" << p1.x << ", " << p1.y << ") => (" << p2.x << ", " << p2.y << ") == " << result);
    return result;
  }

  /**
   * @brief For now, this is going to be all about speed, distance and time
   */
  bool GoalManager::evaluate(const SOLUTION& s, double& cost, double& utility){
    static const int MAX_PRIORITY = 5;
    cost = 0;
    utility = 0;
    TokenId predecessor;
    unsigned int numConflicts(0);
    double pathLength = 0;
    Position currentPosition = getCurrentPosition();

    utility = 0;
    for(SOLUTION::const_iterator it = s.begin(); it != s.end(); ++it){
      TokenId candidate = *it;
      checkError(candidate.isId() && candidate->master().isNoId(), candidate->toString());

      // Utility is 10 to the power of the priority
      utility += pow(10.0, (MAX_PRIORITY - getPriority(candidate)));

      // Check if there is a conflict
      if(predecessor.isId() && !getPlanDatabase()->getTemporalAdvisor()->canPrecede(predecessor, candidate))
	numConflicts++;

      Position nextPosition = getPosition(candidate);

      pathLength += computeDistance(currentPosition, nextPosition);
      predecessor = candidate;
      currentPosition = nextPosition;
    }

    // Priority is to remove conflicts so a much higher weight is given to that
    cost = (pathLength / getSpeed()) + (numConflicts * pow(10.0, MAX_PRIORITY));

    // Finally, feasibility is based on cost being within available budget for time.
    // That budget is defined by the look ahead window of the solver
    return cost <= m_timeBudget;
  }

  std::string GoalManager::toString(const SOLUTION& s){
    double cost, utility;
    bool feasible = evaluate(s, cost, utility);

    std::stringstream ss;
    for(SOLUTION::const_iterator it = s.begin(); it != s.end(); ++it){
      TokenId t = *it;
      ss << t->getKey() << ":";
    }
    ss << "(" << cost << "/" << utility << "), "
       << (feasible ? "feasible." : "infeasible.");
    return ss.str();
  }


  void GoalManager::setState(const State& s){
    if (s != m_state) {
      if (s == STATE_REQUIRE_PLANNING) {
	debugMsg("GoalManager:setState", "Setting state to STATE_REQUIRE_PLANNING.");
      }
      if (s == STATE_PLANNING) {
	debugMsg("GoalManager:setState", "Setting state to STATE_PLANNING.");
      }
      if (s == STATE_DONE) {
	debugMsg("GoalManager:setState", "Setting state to STATE_DONE.");
      }
    }
    m_state = s;
  }


  /**
   * @brief Get the best neighbor for the current solution. The neighborhood is the set of solutions
   * within a single operation from the current solution. The operators are:
   * 1. insert
   * 2. swap
   * 3. remove
   * There are O(n^2) neigbors
   *
   * @note There is alot more we can do to exploit temporal constraints and evaluate feasibility.
   * @todo Cache feasibility of current solution
   */
  void GoalManager::selectNeighbor(GoalManager::SOLUTION& s, TokenId& delta){
    checkError(!m_currentSolution.empty() || !m_ommissions.empty(), "There must be something to do");
    s = m_currentSolution;

    // Feasibility can be used to avoid moves that are silly. We should be able to cache the feasiblility of the current solution
    // rather than compute anew.
    double d1, d2;
    bool feasible = evaluate(m_currentSolution, d1, d2);
    delta = TokenId::noId();

    // Try insertions - could skip if current solution is infeasible.
    if(feasible){
      for(TokenSet::const_iterator it = m_ommissions.begin(); it != m_ommissions.end(); ++it){
	TokenId t = *it;
	for (unsigned int i = 0; i <= m_currentSolution.size(); i++){
	  SOLUTION c = m_currentSolution;
	  insert(c, t, i);
	  update(s, delta, c, t);
	}
      }
    }

    // Swapping is always an option for improving things
    for(unsigned int i=0; i< m_currentSolution.size(); i++){
      for(unsigned int j=i+1; j<m_currentSolution.size(); j++){
	if(i != j){
	  SOLUTION c = m_currentSolution;
	  swap(c, i, j);
	  update(s, delta, c, TokenId::noId());
	}
      }
    }

    // Try removals, assuming it is infeasible
    if(!feasible){
      for(SOLUTION::const_iterator it = m_currentSolution.begin(); it != m_currentSolution.end(); ++it){
	TokenId t = *it;

	if(t->isActive())
	  continue;

	SOLUTION c = m_currentSolution;
	remove(c, t);
	update(s, delta, c, t);
      }
    }
  }

  void GoalManager::insert(SOLUTION& s, const TokenId& t, unsigned int pos){
    checkError(pos <= s.size(), pos << " > " << m_currentSolution.size());

    debugMsg("GoalManager:insert", "Inserting " << t->toString() << " at [" << pos << "] in " << toString(s));

    SOLUTION::iterator it = s.begin();
    for(unsigned int i=0;i<=pos;i++){
      // If we have the insertion point, do it and quit
      if (i == pos){
	s.insert(it, t);
	break;
      }

      // Otherwise advance
      ++it;
    }
  }

  void GoalManager::swap(SOLUTION& s, unsigned int a, unsigned int b){
    debugMsg("GoalManager:swap", "Swapping [" << a << "] and [" << b << "] in " << toString(s));
    SOLUTION::iterator it = s.begin();
    SOLUTION::iterator it_a = s.begin();
    SOLUTION::iterator it_b = s.begin();
    TokenId t_a, t_b;
    unsigned int i = 0;
    unsigned int max_i = std::max(a, b);

    // Compute swap data
    while (i <= max_i){
      TokenId t = *it;
      if (i == a){
	it_a = it;
	t_a = t;
      }
      else if(i == b){
	it_b = it;
	t_b = t;
      }

      i++;
      ++it;
    }

    checkError(a != b && it_a != it_b && t_a != t_b, "Should be different");

    // Execute swap
    it_a = s.erase(it_a);
    s.insert(it_a, t_b);
    it_b = s.erase(it_b);
    s.insert(it_b, t_a);
  }

  void GoalManager::remove(SOLUTION& s, const TokenId& t){
    debugMsg("GoalManager:remove", "Removing " << t->toString() << " from " << toString(s));
    SOLUTION::iterator it = s.begin();
    while((*it) != t && it != s.end()) ++it;
    checkError(it != s.end(), "Not found");
    s.erase(it);
  }

  /**
   * @brief Will promote a solution of equal or better score. This allows a move in a plateau
   */
  void GoalManager::update(SOLUTION& s, TokenId& delta, const SOLUTION& c, TokenId t){
    debugMsg("GoalManager:update", "Evaluating " << toString(c));
    if(compare(c, s) >= 0){
      s = c;
      delta = t;
      debugMsg("GoalManager:update", "Promote " << toString(c) << " >>> " << toString(s));
    } else {
      debugMsg("GoalManager:update", "Not promoting " << toString(c) << " <<< " << toString(s));
    }
  }

  int GoalManager::getPriority(const TokenId& token){
    // Slaves take top priority. Cannot be rejected.
    if(token->master().isId()) {
      debugMsg("GoalManager:Priority", "Token a slave: " << token->toString());
      return 0;
    }

    // Goals with no priority specified are assumed to be priority 0
    ConstrainedVariableId p = token->getVariable(PRIORITY());
    if(p.isNoId()) {
      debugMsg("GoalManager:Priority", "No priority for token: " << token->toString());
      return 0;
    }

    // Now we get the actual priority value
    checkError(p->lastDomain().isSingleton(), p->toString() << " must be set for " << token->toString());
    int pr = (int) p->lastDomain().getSingletonValue();
    debugMsg("GoalManager:Priority", pr << " is priority for: " << token->toString());
    return pr;
  }

  void GoalManager::setInitialConditions(){
    // Start time
    const IntervalIntDomain& horizon = DeliberationFilter::getHorizon();
    m_startTime = (int) horizon.getLowerBound();
    m_timeBudget = (int) (horizon.getUpperBound() - horizon.getLowerBound());
    debugMsg("GoalManager", "Time budget: " << m_timeBudget << " (" <<
	     horizon.getLowerBound() << ", " << horizon.getUpperBound() << ")");
    // Need to get the position value of the token that is spanning the current tick.
  }


  /**
   * @brief is s1 better than s2
   * @return WORSE if s1 < s2. EQUAL if s1 == s2. BETTER if s1 > s2
   */
  int GoalManager::compare(const SOLUTION& s1, const SOLUTION& s2){
    bool f1, f2;
    double c1, c2, u1, u2;
    f1 = evaluate(s1, c1, u1);
    f2 = evaluate(s2, c2, u2);

    // Feasibility is dominant.
    if(!f1) {
      if (c1 < c2) {
	return BETTER;
      } else if (c1 > c2) {
	return WORSE;
      }
      if (u1 > u2) {
	return BETTER;
      } else if (u1 < u2) {
	return WORSE;
      }
    } else {
      if (f2) {
	if (u1 > u2) {
	  return BETTER;
	} else if (u1 < u2) {
	  return WORSE;
	}
	if (c1 < c2) {
	  return BETTER;
	} else if (c1 > c2) {
	  return WORSE;
	}
      }
    }

    return EQUAL;
  }

  Position GoalManager::getPosition(const TokenId& token){
    // Retrieve Variables by parameter name
    ConstrainedVariableId x = token->getVariable(X());
    ConstrainedVariableId y = token->getVariable(Y());

    // Validate token structure for valid position data
    checkError(x.isId(), "No variable for X in token " << token->toString());
    checkError(y.isId(), "No variable for Y in token " << token->toString());
    checkError(x->lastDomain().isSingleton(), x->lastDomain().toString()
	       << ", the X position of " << token->toString() << " is not a singleton.");
    checkError(y->lastDomain().isSingleton(), y->lastDomain().toString()
	       << ", the Y position of " << token->toString() << " is not a singleton.");

    Position p;
    p.x = x->lastDomain().getSingletonValue();
    p.y = y->lastDomain().getSingletonValue();

    return p;
  }

  /**
   * @brief We assume the curent position is a token on a given timeline that contains the current tick, and has x and y as arguments
   * indicating position.
   */
  Position GoalManager::getCurrentPosition() {
    TICK tick = Agent::instance()->getCurrentTick();
    TokenId goodToken = TokenId::noId();
    
    debugMsg("GoalManager:getCurrentPosition", "Looking for position from token, tick: " << tick << ".");

    if(m_positionSource.isId()){
      const std::list<TokenId>& tokens = m_positionSource->getTokenSequence();
      for(std::list<TokenId>::const_iterator it = tokens.begin(); it != tokens.end(); ++it){
	TokenId token = *it;
	debugMsg("GoalManager:getCurrentPosition", "Token range: [" << token->start()->lastDomain().getUpperBound()
		 << ", " << token->end()->lastDomain().getLowerBound() << "].");
	ConstrainedVariableId x = token->getVariable(X());
	ConstrainedVariableId y = token->getVariable(Y());
	checkError(x.isId(), "No variable for X in token " << token->toString());
	checkError(y.isId(), "No variable for Y in token " << token->toString());


	if(x->lastDomain().isSingleton() && y->lastDomain().isSingleton()) {
	  debugMsg("GoalManager:getCurrentPosition", "Token contains usable position info: " << token->toString());
	  goodToken = token;
	} else {
	  debugMsg("GoalManager:getCurrentPosition", "Token is vacuous: " << token->toString());
	}

	// Break if past the current time
	if(token->start()->lastDomain().getLowerBound() > tick)
	  break;
      }
    }
    if (goodToken != TokenId::noId()) {
      m_position = getPosition(goodToken);
    }

    return m_position;
  }

  /**
   * @todo Implement an accessor for speed to compute time bounds. Should be a parameter, ultimately, of the goal
   * but we can assume a nominal speed.
   */
  double GoalManager::getSpeed() const {
    return 1.0;
  }

  /**
   * This stuff not used for now
   */
  CostEstimator::CostEstimator(const TiXmlElement& configData) {
  }
  CostEstimator::~CostEstimator() {
  }

  EuclideanCostEstimator::EuclideanCostEstimator(const TiXmlElement& configData) :
    CostEstimator(configData) {
  }
  EuclideanCostEstimator::~EuclideanCostEstimator() {
  }
  double EuclideanCostEstimator::computeDistance(const Position& p1, const Position& p2) {
    double result = sqrt(pow(p1.x-p2.x, 2) + pow(p1.y-p2.y, 2));
    debugMsg("GoalManager:computeDistance", "Distance between (" << p1.x << ", " << p1.y << ") => (" << p2.x << ", " << p2.y << ") == " << result);
    return result;
  }
}
