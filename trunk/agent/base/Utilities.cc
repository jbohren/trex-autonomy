#include "Utilities.hh"
#include "Agent.hh"
#include "Debug.hh"
#include "DbCore.hh"
#include "UnboundVariableDecisionPoint.hh"
#include "FlawHandler.hh"
#include "Filters.hh"
#include "DbCore.hh"
#include "SimAdapter.hh"
#include "Constraints.hh"
#include "Token.hh"
#include "TokenVariable.hh"
#include <fstream>

using namespace EUROPA;

namespace TREX {
  ConfigurationException::ConfigurationException(const std::string& description) : m_description(description) {}
  const std::string& ConfigurationException::toString() const {
    return m_description;
  }
  void ConfigurationException::configurationCheckError(bool check, const std::string& description) {
    if (!check) {
      debugMsg("trex:error", "Configuration Error: " << description << std::endl);
      throw new ConfigurationException(description);
    }
  }

  std::string timeString(){
    std::stringstream ss;
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    ss << "[" << tv.tv_sec << "." << tv.tv_usec << "]";
    return ss.str();
  }

  const LabelStr& getObjectName(const TokenId& token) {
    checkError(token->getObject()->lastDomain().isSingleton(), "Must be a singleton:" << token->getObject()->toString());
    ObjectId obj = static_cast<ObjectId>(token->getObject()->lastDomain().getSingletonValue());
    return obj->getName();
  }


  TokenId getParentToken(const ConstrainedVariableId& var){
    if(var->parent().isId()){
      if(TokenId::convertable(var->parent()))
	return var->parent();

      if(RuleInstanceId::convertable(var->parent())){
	RuleInstanceId r = var->parent();
	return r->getToken();
      }
    }

    return TokenId::noId();
  }

  void initTREX(){
    // TeleoReactor Factory Bindings for default components
    new TeleoReactor::ConcreteFactory<DbCore>("DeliberativeReactor");
    new TeleoReactor::ConcreteFactory<SimAdapter>("SimAdapter");
  }

  /**
   * @brief Run utility to run a sample problem with a pseudo clock
   */
  void runAgent(const char* configFile, unsigned int stepsPerTick, const char* problemName){
    // PseudoClock uses a sleep duration of 0.0 seconds. This causes the system to run faster for testing
    // and thus it will utilize as much CPU time as is available.
    PseudoClock clock(1.0, stepsPerTick);
    TiXmlElement* root = initXml( findFile(configFile).c_str() );

    Agent::initialize(*root, clock, 0, true);

    while(!Agent::instance()->missionCompleted()){
      Agent::instance()->doNext();
    }

    std::vector<Agent::Event> eventLog = Agent::instance()->getEventLog();
    std::string eventLogStr = Agent::toString(eventLog);

    if(!hasValidFile(problemName)){
      makeValidFile(eventLogStr, problemName);
    }
    else if(!isValid(eventLogStr, problemName)){
      makeInvalidFile(eventLogStr, problemName);
      std::string fname(problemName);
      fname = fname + ".error";
      assertTrue(0, fname); 
    }

    Agent::reset();
  
    delete root;
  };

  bool hasValidFile(const char* problemName){
    std::string fname(problemName);
    fname = fname + ".valid";
    std::ifstream f(findFile(fname).c_str());
    return f.good();
  }

  bool isValid(const std::string& testStr, const char* problemName){
    std::string fname(problemName);
    fname = fname + ".valid";
    std::ifstream f(findFile(fname).c_str());
    assertTrue(f.good(), "There should be a file for " + fname);
    std::stringstream sstr;
    sstr << testStr;
    bool OK = true;
    while(f.good() && !f.eof() && OK){
      std::string validStr, candidateStr;
      getline(f, validStr);
      getline(sstr, candidateStr);
      if(validStr != candidateStr){
	debugMsg("Test:", candidateStr << " != " << validStr);
	OK = false;
      }
    }

    return OK;
  }

  void makeValidFile(const std::string& validStr, const char* problemName){
    makeFile(validStr, problemName, "valid");
  }

  void makeInvalidFile(const std::string& validStr, const char* problemName){
    makeFile(validStr, problemName, "error");
  }

  void makeFile(const std::string& outputStr, const char* prefix, const char* suffix){
    std::string fname(prefix);
    fname = fname + ".";
    fname = fname + suffix;
    std::ofstream f(fname.c_str());
    f << outputStr;
    f.close();
  }

  std::string findFile(const std::string& fileName){
    static std::vector<std::string> sl_locations;

    if(sl_locations.empty()){
      sl_locations.push_back("./");

      char * path = getenv("TREX_PATH");
      if(path != NULL){
	LabelStr pathLblStr(path);
	unsigned int count = pathLblStr.countElements(":");
	for(unsigned int i = 0; i < count; i++){
	  sl_locations.push_back(pathLblStr.getElement(i, ":").toString() + "/");
	}
      }
    }


    //TREX_INFO("findfile", "Searching for " << fileName << ".\n");
    //printf("Searching for %s.\n", fileName.c_str());

    const char * start = getenv("TREX_START_DIR");


    if (start) {
      char * path2 = (char*)malloc(sizeof(char) * (strlen(start) + 2));
      memset(path2, 0, sizeof(char) * (strlen(start) + 2));
      memcpy((void*)path2, (const void*)start, sizeof(char) * strlen(start));


      //TREX_INFO("findfile", "Starting search in " << path2 << ".\n");
      //printf("Starting search in %s.\n", path2);

      while(strlen(path2) != 0) {
	//TREX_INFO("findfile", "\tIn " << path2 << ".\n");
	//printf("\tIn %s.\n", path2);
	std::string qualifiedFileName = path2 + std::string("/") + fileName;
	
	//Check if f exists.
	std::ifstream f(qualifiedFileName.c_str());
	if(f.good()) {
	  f.close();
	  return qualifiedFileName;
	}
	
	//Check if we've intersected the trex path.
	bool already_path = false;
	for(std::vector<std::string>::const_iterator it = sl_locations.begin(); it != sl_locations.end(); ++it){
	  if ((std::string(path2) + std::string("/")) == *it) {
	    already_path = true;
	    break;
	  }
	}
	if (already_path) {
	  //TREX_INFO("findfile", "Already in the path at " << path2 << ", bailing.\n");
	  break;
	}
	
	

	//Delete a directory from the end, to move up a directory.
	if (strlen(path2) <= 1) {
	  break;
	}
	unsigned int i = strlen(path2) - 1;
	if (path2[i] == '/' || path2[i] == '\\') {
	  path2[i] = 0;
	  i--;
	}



	do {
	  i--;
	  if (path2[i] == '/' || path2[i] == '\\') {
	    path2[i] = 0;
	    break;
	  }
	  path2[i] = 0;
	} while (i != 0);
      }
    }



    std::string qualifiedFileName = "";

    // Search the path in order
    for(std::vector<std::string>::const_iterator it = sl_locations.begin(); it != sl_locations.end(); ++it){
      const std::string& path = *it;
      std::string qualifiedFileName = path + fileName;
      std::ifstream f(qualifiedFileName.c_str());
      if(f.good()) {
	f.close();
	return qualifiedFileName;
      }
    }

    return fileName;
  }

  std::vector<ConstrainedVariableId> appendStateVar(const std::vector<ConstrainedVariableId>& variables){
    // It can be the case that when we are copying a constraint, for example when merging, that the state variable will already have
    // been added. Thus we check the scope and only append if we have to
    std::vector<ConstrainedVariableId> newScope(variables);
    TokenId token = getParentToken(variables[0]);
    for(std::vector<ConstrainedVariableId>::const_iterator it = newScope.begin(); it != newScope.end(); ++it){
      if(*it == token->getState())
	return newScope;
    }

    newScope.push_back(token->getState());
    return newScope;
  }

  SetDefault::SetDefault(const LabelStr& name,
			 const LabelStr& propagatorName,
			 const ConstraintEngineId& constraintEngine,
			 const std::vector<ConstrainedVariableId>& variables)
    : Constraint(name, propagatorName, constraintEngine, appendStateVar(variables)),
      m_token(getParentToken(m_variables[0])),
      m_param(getCurrentDomain(m_variables[0])),
      m_default(getCurrentDomain(m_variables[1])){
    checkError(m_token.isValid(), m_token);
  }

  /**
   * @brief If the token is active and the parameter of interest is not set, then apply the default. This is legal 
   * during propagation because committing a token is monotonic.
   */
  bool SetDefault::defaultGuardSatisfied(){
    return m_token->isActive() && !m_param.isSingleton() && m_default.isSingleton();
  }

  void SetDefault::setSource(const ConstraintId& constraint){
    SetDefault* c = (SetDefault*) constraint;
    m_token = c->m_token;
  }

  void SetDefault::handleExecute() {
    if(!defaultGuardSatisfied())
      return;

    if(m_param.intersects(m_default))
	m_param.intersect(m_default);
    else if(m_param.isNumeric()){
      double defaultValue = m_default.getSingletonValue();
      double deltaUB = fabs(m_param.getUpperBound() - defaultValue);
      double deltaLB = fabs(m_param.getLowerBound() - defaultValue);
      if(deltaUB < deltaLB)
	m_param.intersect(m_param.getUpperBound(), m_param.getUpperBound());
      else
	m_param.intersect(m_param.getLowerBound(), m_param.getLowerBound());
    }
  }

  SetDefaultOnCommit::SetDefaultOnCommit(const LabelStr& name,
				       const LabelStr& propagatorName,
				       const ConstraintEngineId& constraintEngine,
				       const std::vector<ConstrainedVariableId>& variables)
    : SetDefault(name, propagatorName, constraintEngine, variables){}

  /**
   * @brief If the token is committed and the parameter of interest is not set, then apply the default. This is legal 
   * during propagation because committing a token is monotonic.
   */
  bool SetDefaultOnCommit::defaultGuardSatisfied(){
    checkError(m_token.isValid(), m_token);
    return m_token->isCommitted();
  }

  /*************************************************************************
   * AbsMaxOnCommit.
   *************************************************************************/
  AbsMaxOnCommit::AbsMaxOnCommit(const LabelStr& name,
				 const LabelStr& propagatorName,
				 const ConstraintEngineId& constraintEngine,
				 const std::vector<ConstrainedVariableId>& variables)
    : Constraint(name, propagatorName, constraintEngine, appendStateVar(variables)),
      m_token(getParentToken(m_variables[0])),
      m_param(getCurrentDomain(m_variables[0])){
    checkError(m_token.isValid(), m_token);
    checkError(m_param.isNumeric(), toString() << " only valid for numeric variables.");
  }

  void AbsMaxOnCommit::setSource(const ConstraintId& constraint){
    AbsMaxOnCommit* c = (AbsMaxOnCommit*) constraint;
    m_token = c->m_token;
  }

  void AbsMaxOnCommit::handleExecute() {
    checkError(m_token.isValid(), m_token);

    if(!m_token->isCommitted() || m_param.isSingleton())
      return;

    if(fabs(m_param.getUpperBound()) >= fabs(m_param.getLowerBound()))
      m_param.set(m_param.getUpperBound());
    else
      m_param.set(m_param.getUpperBound());
  }

  BindMax::BindMax(const LabelStr& name,
		     const LabelStr& propagatorName,
		     const ConstraintEngineId& constraintEngine,
		     const std::vector<ConstrainedVariableId>& variables)
    : Constraint(name, propagatorName, constraintEngine, variables),
      m_target(getCurrentDomain(m_variables[0])),
      m_source(getCurrentDomain(m_variables[1])){
    checkError(m_variables.size() == 2, "Exactly 2 parameters required. ");
  }

  void BindMax::handleExecute(){
    double maxValue = m_source.getUpperBound();
    double oldLB =  m_target.getLowerBound();
    double oldUB = m_target.getUpperBound();

    if(m_target.isMember(maxValue))
      m_target.set(maxValue);
    else if(maxValue < oldLB)
      m_target.set(oldLB);
    else if(maxValue > oldUB)
      m_target.set(oldUB);
  }

  Neighborhood::Neighborhood(const LabelStr& name,
		     const LabelStr& propagatorName,
		     const ConstraintEngineId& constraintEngine,
		     const std::vector<ConstrainedVariableId>& variables)
    : Constraint(name, propagatorName, constraintEngine, variables),
      m_element(getCurrentDomain(m_variables[0])),
      m_cardinality(getCurrentDomain(m_variables[1])),
      m_neighborhood(getCurrentDomain(m_variables[2])),
      m_position(PLUS_INFINITY){
    checkError(m_variables.size() == 3, "Exactly 3 parameters required. ");
    std::list<double> vals;
    m_variables[2]->baseDomain().getValues(vals);
    for(std::list<double>::const_iterator it = vals.begin(); it != vals.end(); ++it)
      m_basis.push_back(*it);

  }

  void Neighborhood::handleExecute(){
    if(!m_element.isSingleton() || !m_cardinality.isSingleton())
      return;

    checkError(m_basis.size() > m_cardinality.getSingletonValue(), 
	       "Invalid parameters:" << m_cardinality.getSingletonValue() << " >= " << m_basis.size());

    // Advance to the element if we do not yet have the position
    unsigned int i = 0;
    if(m_position == (unsigned int) PLUS_INFINITY){
      for(std::vector<double>::const_iterator it = m_basis.begin(); it != m_basis.end(); ++it){
	if(*it == m_element.getSingletonValue()){
	  m_position = i;
	  break;
	}
	i++;
      }
    }

    // Now compute the entries by getting the successor elements, wrapping around
    checkError(m_position < m_basis.size(), "Element not found");
    unsigned int cardinality = (unsigned int) m_cardinality.getSingletonValue();
    EnumeratedDomain dom(m_neighborhood.getDataType());
    for(i = 1; i <= cardinality; i++)
      dom.insert(m_basis[(m_position + i) % m_basis.size()]);
    dom.close();
    m_neighborhood.intersect(dom);
  }
}
