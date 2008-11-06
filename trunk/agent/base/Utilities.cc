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

  const LabelStr& getObjectName(const TokenId& token) {
    checkError(token->getObject()->lastDomain().isSingleton(), "Must be a singleton:" << token->getObject()->toString());
    ObjectId obj = static_cast<ObjectId>(token->getObject()->lastDomain().getSingletonValue());
    return obj->getName();
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
    // Keep the sleep seconds tiny since we want to run tests quickly. Higher values yield lower cpu utilization
    // but also affect the likelihood of missing a tick. For testing it should be basically 0.
    PseudoClock clock(0.0, stepsPerTick);
    TiXmlElement* root = initXml( findFile(configFile).c_str() );

    Agent::initialize(*root, clock);

    while(!Agent::instance()->missionCompleted()){
      Agent::instance()->doNext();
    }

    std::vector<Agent::Event> eventLog = Agent::instance()->getEventLog();
    std::string eventLogStr = Agent::toString(eventLog);

    if(!hasValidFile(problemName) || !isValid(eventLogStr, problemName)){
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
    std::stringstream ss;
    ss << "GENERATED FILE. VERIFY AND CLEAN" << std::endl << validStr;
    makeFile(ss.str(), problemName, "valid");
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

    std::string qualifiedFileName = "";

    // Search the path in order
    for(std::vector<std::string>::const_iterator it = sl_locations.begin(); it != sl_locations.end(); ++it){
      const std::string& path = *it;
      std::string qualifiedFileName = path + fileName;
      std::ifstream f(qualifiedFileName.c_str());
      if(f.good())
	return qualifiedFileName;
    }

    return fileName;
  }

  SetDefault::SetDefault(const LabelStr& name,
			 const LabelStr& propagatorName,
			 const ConstraintEngineId& constraintEngine,
			 const std::vector<ConstrainedVariableId>& variables)
    : Constraint(name, propagatorName, constraintEngine, appendStateVar(variables)),
      m_token(DbCore::getParentToken(m_variables[0])),
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

    handleCompletion();
  }


  std::vector<ConstrainedVariableId> SetDefault::appendStateVar(const std::vector<ConstrainedVariableId>& variables){
    // It can be the case that when we are copying a constraint, for example when merging, that the state variable will already have
    // been added. Thus we check the scope and only append if we have to
    std::vector<ConstrainedVariableId> newScope(variables);
    if(newScope.size() == 2){
      TokenId token = DbCore::getParentToken(variables[0]);
      newScope.push_back(token->getState());
    }

    return newScope;
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

  void SetDefaultOnCommit::handleCompletion(){
    m_variables[0]->restrictBaseDomain(m_param);
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
    EnumeratedDomain dom(m_neighborhood.isNumeric(), m_neighborhood.getTypeName().c_str());
    for(i = 1; i <= cardinality; i++)
      dom.insert(m_basis[(m_position + i) % m_basis.size()]);
    dom.close();
    m_neighborhood.intersect(dom);
  }
}
