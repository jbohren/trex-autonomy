#ifdef __BEOS__
#include <Path.h>
#endif

#include "DbWriter.hh"

#include "Constraint.hh"
#include "ConstraintEngine.hh"
#include "ConstraintEngineDefs.hh"
#include "ConstrainedVariable.hh"
#include "Domains.hh"
#include "LabelStr.hh"
#include "Variable.hh"

#include "PlanDatabase.hh"
#include "PlanDatabaseDefs.hh"
#include "Object.hh"
#include "Timeline.hh"
#include "Token.hh"
#include "TokenVariable.hh"

#include "Rule.hh"
#include "RulesEngine.hh"
#include "RulesEngineDefs.hh"
#include "RuleInstance.hh"

#include "Debug.hh"

#include "tinyxml.h"

#include <algorithm>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <stdexcept>
#include <set>
#include <typeinfo>
#include <vector>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>

#define FatalError(cond, msg...){Error(cond, msg, __FILE__, __LINE__).handleAssert();}
#define FatalErrno(){FatalError("Condition", strerror(errno))}
#define FatalErr(s) {std::cerr << (s) << std::endl; FatalErrno(); }

#define IN_NO_SECTION 0
#define IN_GENERAL_SECTION 1

#ifdef __MINGW32__ 
	static int mkdir(const char* path, mode_t mode) { 
		int toRet = mkdir(path); 
		chmod(path, mode);
		return toRet;
	}
#endif

namespace TREX {

  inline long long int timeval2Id(const struct timeval &currTime) {
    return (((long long int) currTime.tv_sec) * 1000) + (currTime.tv_usec / 1000);
  }

  const std::string DEST("plans");
  const char *envPPWConfigFile = "PPW_CONFIG";
  const std::string DURATION_VAR("DURATION_VAR");
  const std::string END_VAR("END_VAR");
  const std::string START_VAR("START_VAR");
  const std::string STATE_VAR("STATE_VAR");
  const std::string OBJECT_VAR("OBJECT_VAR");
  const std::string PARAMETER_VAR("PARAMETER_VAR");
  const std::string MEMBER_VAR("MEMBER_VAR");
  const std::string RULE_VAR("RULE_VAR");

  const std::string tokenVarTypes[8] = 
    {STATE_VAR, OBJECT_VAR, DURATION_VAR, START_VAR, END_VAR, PARAMETER_VAR, MEMBER_VAR, RULE_VAR};

  enum varTypes {I_STATE = 0, I_OBJECT, I_DURATION, I_START, I_END, I_PARAMETER, I_MEMBER, I_RULE};
  enum objectTypes {O_OBJECT = 0, O_TIMELINE, O_RESOURCE};
  enum tokenTypes {T_INTERVAL = 0, T_TRANSACTION};
  enum decisionTypes {D_OBJECT = 0, D_TOKEN, D_VARIABLE, D_RESOURCE, D_ERROR};

#define SEQ_COL_SEP (unsigned char) 0x1e
#define SEQ_LINE_SEP (unsigned char) 0x1f
  const std::string TAB("\t");
  const std::string COLON(":");
  const std::string SNULL("\\N");
  const std::string CONSTRAINT_TOKEN("constraintToken");
  const std::string COMMA(",");
  const std::string SLASH("/");
  const std::string SPACE(" ");
  const std::string TEMPORAL("TEMPORAL");
  const std::string ATEMPORAL("ATEMPORAL");
  const std::string VAR_TEMP_CONSTR("variableTempConstr");
  const std::string UNARY_TEMP_CONSTR("unaryTempConstr");
  const std::string FIXED_TEMP_CONSTR("fixedTempConstr");
  const std::string UNARY_CONSTR("unaryConstr");
  const std::string EQ_CONSTR("equalityConstr");
  const std::string BUG_CONSTR("bugConstr");
  const std::string STRUE("true");
  const std::string SFALSE("false");
  const std::string PINFINITY("Infinity");
  const std::string NINFINITY("-Infinity");
  const std::string INTEGER_SORT("INTEGER_SORT");
  const std::string REAL_SORT("REAL_SORT");
  const std::string STEP("plan");
  const std::string PARTIAL_PLAN_STATS("/partialPlanStats");
  const std::string SEQUENCE("/sequence");
  const std::string RULES("/rules");
  const std::string PARTIAL_PLAN(".partialPlan");
  const std::string OBJECTS(".objects");
  const std::string TOKENS(".tokens");
  const std::string RULE_INSTANCES(".ruleInstances");
  const std::string RULE_INSTANCE_SLAVE_MAP(".ruleInstanceSlaveMap");
  const std::string VARIABLES(".variables");
  const std::string CONSTRAINTS(".constraints");
  const std::string CONSTRAINT_VAR_MAP(".constraintVarMap");
  const std::string INSTANTS(".instants");
  const std::string DECISIONS(".decisions");
  const std::string E_DOMAIN("E");
  const std::string I_DOMAIN("I");
  const std::string CAUSAL("CAUSAL");
  const std::string ENUM_DOMAIN("EnumeratedDomain");
  const std::string INT_DOMAIN("IntervalDomain");
  const std::string ROOT_CONFIG("DbWriterConfig");
  const std::string GENERAL_CONFIG_SECTION("GeneralConfigSection");
  const std::string RULE_CONFIG_SECTION("RuleConfigSection");
  const std::string SOURCE_PATH("SourcePath");
  const std::string AUTO_WRITE("AutoWrite");
  const std::string STEPS_PER_WRITE("StepsPerWrite");
  const std::string WRITE_FINAL_STEP("WriteFinalStep");
  const std::string WRITE_DEST("WriteDest");
  const std::string MAX_CHOICES("MaxChoices");

  const std::string configSections[] = {GENERAL_CONFIG_SECTION, RULE_CONFIG_SECTION};

#ifdef __BEOS__
#define NBBY 8
  static char *realpath(const char *path, char *resolved_path) {
    BPath tempPath(path,NULL,true);
    if (tempPath.Path() == NULL) {
      return NULL;
    }
    strcpy(resolved_path,tempPath.Path());
    return resolved_path;
  }
#endif

#ifdef __MINGW32__
 
#define NBBY 8
			
  static char *realpath(const char *path, char *resolved_path) {
    char* temp;
    if (GetFullPathNameA(path, MAXPATHLEN, resolved_path, &temp) == 0)
      return NULL;
    return resolved_path;
  }
#endif

  DbWriter::DbWriter(const std::string& agentName, const std::string& reactorName, 
		     const PlanDatabaseId& db, const ConstraintEngineId& ce, const RulesEngineId &re)
    : m_agentName(agentName),
      m_reactorName(reactorName),
      dest(TREX::LogManager::instance().reactor_dir_path(agentName,reactorName,"plans")),
      pdbId(db),
      ceId(ce), 
      reId(re), 
      stepCount(0),
      destAlreadyInitialized(false), 
      m_writing(false){
    //add default directories to search for model files
    sourcePaths.push_back("");
    sourcePaths.push_back(".");
    sourcePaths.push_back("..");

    struct timeval currTime;
    if(gettimeofday(&currTime, NULL)) {
      FatalError("gettimeofday()", "Failed to get current time.");
    }

    seqId = timeval2Id(currTime);
  }

  DbWriter::~DbWriter(void) {
    if(destAlreadyInitialized) {
      statsOut->close();
      delete statsOut;
    }
  }

  void DbWriter::writeStatistics(void) {
    writeStats();
  }

  void DbWriter::writeStats(void) {
    if(!destAlreadyInitialized) {
      initOutputDestination();
      destAlreadyInitialized = true;
    }

    ppId = 0LL;
    collectStats();
    debugMsg("DbWriter:writeStats", "Writing statistics numTokens: " << numTokens 
	     << " numVariables: " << numVariables << " numConstraints: " << numConstraints);
    (*statsOut) << seqId << TAB << ppId << TAB << stepCount << TAB << numTokens << TAB << numVariables
		<< TAB << numConstraints << std::endl;
    statsOut->flush();

  }


  /* collects all but numTransactions which are
     collected incrementally
  */
  void DbWriter::collectStats(void) {
    TokenSet tokens(pdbId->getTokens());
    numTokens = tokens.size();
    ConstrainedVariableSet variables = ceId->getVariables();
    numVariables = variables.size();
    const ConstraintSet &constraints = ceId->getConstraints();
    numConstraints = constraints.size();
  }

  void DbWriter::outputObject(const ObjectId &objId, const int type,
			      std::ofstream &objOut, std::ofstream &varOut) {
    int parentKey = -1;
    if(!objId->getParent().isNoId())
      parentKey = objId->getParent()->getKey();
    objOut << objId->getKey() << TAB << type << TAB << parentKey << TAB
	   << ppId << TAB << objId->getName().toString() << TAB;
    /*ChildObjectIds*/
    if(objId->getComponents().empty()) {
      objOut << SNULL << TAB;
    }
    else {
      for(ObjectSet::const_iterator childIt = objId->getComponents().begin();
	  childIt != objId->getComponents().end(); ++childIt) {
	ObjectId child = *childIt;
	objOut << child->getKey() << COMMA;
      }
      objOut << TAB;
    }
    /*end ChildObjectIds*/
    /*VariableIds*/
    if(objId->getVariables().empty()) {
      objOut << SNULL << TAB;
    }
    else {
      for(std::vector<ConstrainedVariableId>::const_iterator varIt = 
	    objId->getVariables().begin(); varIt != objId->getVariables().end(); ++varIt) {
	ConstrainedVariableId var = *varIt;
	objOut << var->getKey() << COMMA;
	outputConstrVar(var, objId->getKey(), I_MEMBER, varOut);
      }
      objOut << TAB;
    }
    /*end VariableIds*/
    /*TokenIds*/
    if(objId->tokens().empty()) {
      objOut << SNULL << TAB;
    }
    else {
      for(TokenSet::const_iterator tokIt = objId->tokens().begin();
	  tokIt != objId->tokens().end(); ++tokIt) {
	TokenId token = *tokIt;
	objOut << token->getKey() << COMMA;
      }
      objOut << TAB;
    }
    /*end TokenIds*/
  }

  void DbWriter::outputToken(const TokenId &token, const int type, const int slotId, 
			     const int slotIndex, const int slotOrder, 
			     const ObjectId &tId, std::ofstream &tokOut, 
			     std::ofstream &varOut) {
    check_error(token.isValid());
    if(token->isIncomplete()) {
      std::cerr << "Token " << token->getKey() << " is incomplete.  Skipping. " << std::endl;
      return;
    }
    if(!tId.isNoId()) {
      tokOut << token->getKey() << TAB << type << TAB << slotId << TAB << slotIndex << TAB 
	     << ppId << TAB << 0 << TAB << 1 << TAB << token->start()->getKey() << TAB 
	     << token->end()->getKey() << TAB << token->duration()->getKey() << TAB 
	     << token->getState()->getKey() << TAB << token->getPredicateName().toString() 
	     << TAB << tId->getKey() << TAB << tId->getName().toString() << TAB 
	     << token->getObject()->getKey() << TAB;
    }
    else {
      tokOut << token->getKey() << TAB << type << TAB << SNULL << TAB << SNULL << TAB << ppId 
	     << TAB << 1 << TAB << 1 << TAB << token->start()->getKey() << TAB 
	     << token->end()->getKey() << TAB << token->duration()->getKey() << TAB 
	     << token->getState()->getKey() << TAB << token->getPredicateName().toString() 
	     << TAB << SNULL << TAB << SNULL << TAB << token->getObject()->getKey() << TAB;
    }
    outputObjVar(token->getObject(), token->getKey(), I_OBJECT, varOut);
    outputIntIntVar(token->start(), token->getKey(), I_START, varOut);
    outputIntIntVar(token->end(), token->getKey(), I_END, varOut);
    outputIntIntVar(token->duration(), token->getKey(), I_DURATION, varOut);
    //outputEnumVar(token->getState(), token->getKey(), I_STATE, varOut);
    outputStateVar(token->getState(), token->getKey(), I_STATE, varOut);

    std::string paramVarIds;
    char paramIdStr[NBBY * sizeof(int) * 28/93 + 4];
    for(std::vector<ConstrainedVariableId>::const_iterator paramVarIterator = 
	  token->parameters().begin();
	paramVarIterator != token->parameters().end(); ++paramVarIterator) {
      ConstrainedVariableId varId = *paramVarIterator;
      check_error(varId.isValid());
      outputConstrVar(varId, token->getKey(), I_PARAMETER, varOut);
      memset(paramIdStr, '\0', NBBY * sizeof(int) * 28/93 + 4);
      sprintf(paramIdStr, "%d", varId->getKey());
      paramVarIds += std::string(paramIdStr) + COLON;
    }
    if(paramVarIds == "") {
      tokOut << SNULL << TAB;
    }
    else {
      tokOut << paramVarIds << TAB;
    }

    /*ExtraInfo: SlotOrder*/
    if(type != T_TRANSACTION) {
      tokOut << slotOrder;
    }
#ifndef NO_RESOURCES
    /* TODO JRB: move this to Resource module
    //ExtraInfo: QuantityMin:QuantityMax
    else{
    TransactionId trans = (TransactionId) token;
    tokOut << trans->getMin() << COMMA << trans->getMax();
    }
    */
#endif
      
    tokOut << std::endl;
  }
  
  void DbWriter::outputStateVar(const Id<TokenVariable<StateDomain> >& stateVar,
				const int parentId, const int type,
				std::ofstream &varOut) {
	
    varOut << stateVar->getKey() << TAB << ppId << TAB << parentId << TAB 
	   << stateVar->getName().toString() << TAB;

    varOut << ENUM_DOMAIN << TAB;
    StateDomain &dom = (StateDomain &) stateVar->lastDomain();
    std::list<double> vals;
    dom.getValues(vals);
    for(std::list<double>::const_iterator it = vals.begin(); it != vals.end(); ++it){
      LabelStr strVal(*it);
      varOut << strVal.toString() << " ";
    }
    varOut << TAB;
    varOut << SNULL << TAB << SNULL << TAB << SNULL << TAB;

    varOut << tokenVarTypes[type] << std::endl;
  }

  void DbWriter::outputEnumVar(const Id<TokenVariable<EnumeratedDomain> >& enumVar, 
			       const int parentId, const int type,
			       std::ofstream &varOut) {
	
    varOut << enumVar->getKey() << TAB << ppId << TAB << parentId << TAB 
	   << enumVar->getName().toString() << TAB;

    varOut << ENUM_DOMAIN << TAB;

    varOut << getEnumerationStr((EnumeratedDomain &)enumVar->lastDomain()) << TAB;
    varOut << SNULL << TAB << SNULL << TAB << SNULL << TAB;

    varOut << tokenVarTypes[type] << std::endl;
  }
  
  void DbWriter::outputIntVar(const Id<TokenVariable<IntervalDomain> >& intVar,
			      const int parentId, const int type,
			      std::ofstream &varOut) {
	
    varOut << intVar->getKey() << TAB << ppId << TAB << parentId << TAB 
	   << intVar->getName().toString() << TAB;

    varOut << INT_DOMAIN << TAB << SNULL << TAB << REAL_SORT << TAB 
	   << getLowerBoundStr((IntervalDomain &)intVar->lastDomain()) << TAB
	   << getUpperBoundStr((IntervalDomain &)intVar->lastDomain()) << TAB;

    varOut << tokenVarTypes[type] << std::endl;
  }
  
  void DbWriter::outputIntIntVar(const Id<TokenVariable<IntervalIntDomain> >& intVar,
				 const int parentId, const int type,
				 std::ofstream &varOut) {
	

    varOut << intVar->getKey() << TAB << ppId << TAB << parentId << TAB 
	   << intVar->getName().toString() << TAB;

    varOut << INT_DOMAIN << TAB << SNULL << TAB << INTEGER_SORT << TAB 
	   << getLowerBoundStr((IntervalDomain &)intVar->lastDomain()) << TAB
	   << getUpperBoundStr((IntervalDomain &)intVar->lastDomain()) << TAB;
    
    varOut << tokenVarTypes[type] << std::endl;
  }

  void DbWriter::outputObjVar(const ObjectVarId& objVar,
			      const int parentId, const int type,
			      std::ofstream &varOut) {
	

    varOut << objVar->getKey() << TAB << ppId << TAB << parentId << TAB 
	   << objVar->getName().toString() << TAB;

    varOut << ENUM_DOMAIN << TAB;

    std::list<double> objects;
    ((ObjectDomain &)objVar->lastDomain()).getValues(objects);
    for(std::list<double>::iterator it = objects.begin(); it != objects.end(); ++it) {
      varOut << ((ObjectId)(*it))->getName().toString() << " ";
    }

    varOut << TAB << SNULL << TAB << SNULL << TAB << SNULL << TAB;

    varOut << tokenVarTypes[type] << std::endl;
  }
  
  void DbWriter::outputConstrVar(const ConstrainedVariableId &otherVar, 
				 const int parentId, const int type,
				 std::ofstream &varOut) {
	

    varOut << otherVar->getKey() << TAB << ppId << TAB << parentId << TAB 
	   << otherVar->getName().toString() << TAB;

    if(otherVar->lastDomain().isEnumerated()) {
      varOut << ENUM_DOMAIN << TAB << getEnumerationStr((EnumeratedDomain &)otherVar->lastDomain()) 
	     << TAB << SNULL << TAB << SNULL << TAB << SNULL << TAB;
    }
    else if(otherVar->lastDomain().isInterval()) {
      varOut << INT_DOMAIN << TAB << SNULL << TAB << REAL_SORT << TAB 
	     << getLowerBoundStr((IntervalDomain &)otherVar->lastDomain()) << TAB 
	     << getUpperBoundStr((IntervalDomain &)otherVar->lastDomain()) << TAB;
    }
    else {
      FatalError("otherVar->lastDomain()isEnumerated() || otherVar->lastDomain().isInterval()",
		 "I don't know what my domain is!");
    }
    varOut << tokenVarTypes[type] << std::endl;
  }

  void DbWriter::outputConstraint(const ConstraintId &constrId, std::ofstream &constrOut, 
				  std::ofstream &cvmOut) {
    constrOut << constrId->getKey() << TAB << ppId << TAB << constrId->getName().toString() 
	      << TAB << ATEMPORAL << std::endl;
    std::vector<ConstrainedVariableId>::const_iterator it =
      constrId->getScope().begin();
    for(; it != constrId->getScope().end(); ++it) {
      cvmOut << constrId->getKey() << TAB << (*it)->getKey() << TAB << ppId << std::endl;
    }
  }

  void DbWriter::outputRuleInstance(const RuleInstanceId &ruleId,
				    std::ofstream &ruleInstanceOut,
				    std::ofstream &varOut,
				    std::ofstream &rismOut) {

    ruleInstanceOut << ruleId->getKey() << TAB << ppId << TAB << seqId
		    << TAB << ruleId->getRule()->getName().toString()
		    << TAB << ruleId->getToken()->getKey() << TAB;

    /*SlaveTokenIds*/
    std::set<TokenId> slaves;
    std::set<ConstrainedVariableId> vars;
    buildSlaveAndVarSets(slaves, vars, ruleId);
    if(slaves.empty()) {
      ruleInstanceOut << SNULL << TAB;
    }
    else {
      for(std::set<TokenId>::const_iterator it = slaves.begin();
	  it != slaves.end(); ++it) {
	TokenId slaveToken = *it;
	if(slaveToken.isValid())
	  ruleInstanceOut << slaveToken->getKey() << COMMA;
	rismOut << ruleId->getKey() <<TAB << slaveToken->getKey() << TAB << ppId << std::endl;
      }
      ruleInstanceOut << TAB;
    }

    /* gaurd and local variables */
    if(vars.empty()) {
      ruleInstanceOut << SNULL;
    }
    else {
      for(std::set<ConstrainedVariableId>::const_iterator it = vars.begin();
	  it != vars.end(); ++it) {
	ConstrainedVariableId localVar = *it;
	ruleInstanceOut << localVar->getKey() << COMMA;
	outputConstrVar(localVar, ruleId->getKey(), I_RULE, varOut);
      }
    }
    ruleInstanceOut << std::endl;
  }

  void DbWriter::buildSlaveAndVarSets(std::set<TokenId> &tokSet,
				      std::set<ConstrainedVariableId> &varSet,
				      const RuleInstanceId &ruleId) {
    std::vector<TokenId> tokVec = ruleId->getSlaves();
    for(std::vector<TokenId>::const_iterator tempIt = tokVec.begin(); tempIt != tokVec.end(); ++tempIt)
      if(!((*tempIt).isNoId()))
	tokSet.insert(*tempIt);

    for(std::vector<ConstrainedVariableId>::const_iterator varIt = ruleId->getVariables().begin();
	varIt != ruleId->getVariables().end(); ++varIt) {
      ConstrainedVariableId var = *varIt;
      if(RuleInstanceId::convertable(var->parent())) {
	varSet.insert(var);
      }
    }
    for(std::vector<RuleInstanceId>::const_iterator ruleIt = ruleId->getChildRules().begin();
	ruleIt != ruleId->getChildRules().end(); ++ruleIt) {
      RuleInstanceId rid = *ruleIt;
      buildSlaveAndVarSets(tokSet, varSet, rid);
    }
  }

  const std::string DbWriter::getUpperBoundStr(IntervalDomain &dom) const {
    if(dom.isNumeric()) {
      if((int) dom.getUpperBound() == PLUS_INFINITY)
	return PINFINITY;
      else if((int) dom.getUpperBound() == MINUS_INFINITY)
	return NINFINITY;
      else {
	std::stringstream stream;
	stream <<  dom.getUpperBound();
	return std::string(stream.str());
      }
    }
    else if(LabelStr::isString((int)dom.getUpperBound())) {
      LabelStr label((int)dom.getUpperBound());
      return label.toString();
    }
    else if(dom.isBool()) {
      if (dom.getUpperBound() == 0)
	return std::string("false");
      return std::string("true");
    }
    else {
      return ObjectId(dom.getUpperBound())->getName().toString();
    }
    return std::string("");
  }

  const std::string DbWriter::getLowerBoundStr(IntervalDomain &dom) const {
      
    if(dom.isNumeric()) {
      if((int)dom.getLowerBound() == PLUS_INFINITY)
	return PINFINITY;
      else if((int) dom.getLowerBound() == MINUS_INFINITY)
	return NINFINITY;
      std::stringstream stream;
      stream << dom.getLowerBound();
      return std::string(stream.str());
    }
    else if(LabelStr::isString((int)dom.getLowerBound())) {
      LabelStr label((int)dom.getLowerBound());
      return label.toString();
    }
    else if(dom.isBool()) {
      if (dom.getLowerBound() == 0)
	return std::string("false");
      return std::string("true");
    }
    else {
      return ObjectId(dom.getLowerBound())->getName().toString();
    }
    return std::string("");
  }
  
  const std::string DbWriter::getEnumerationStr(EnumeratedDomain &edom) const {
    std::stringstream stream;
    std::list<double> enumeration;
    EnumeratedDomain dom(edom);
    if(dom.isOpen()) {
      dom.close();
    }
    if(dom.isInfinite()) {
      return "-Infinity +Infinity";
    }
    if(dom.isEmpty()) {
      return "empty";
    }
    else {
      dom.getValues(enumeration);
    }
    for(std::list<double>::iterator it = enumeration.begin(); it != enumeration.end(); ++it) {
      if(dom.isNumeric()) {
	if((int) (*it) == PLUS_INFINITY)
	  stream << PINFINITY;
	else if((int) (*it) == MINUS_INFINITY)
	  stream << NINFINITY;
	else
	  stream << (int)(*it) << " ";
      }
      else if(LabelStr::isString(*it)) {
	LabelStr label(*it);
	stream << label.toString() << " ";
      }
      else if(dom.isBool()) {
	if ((*it) == 0)
	  stream << "false" << " ";
	stream << "true" << " ";
      }
      else {
	stream << ObjectId(*it)->getName().toString() << " ";
      }
    }

    //BUG: WHAT IS THIS
    //if(streamIsEmpty(stream)) {
    //  return "bugStr";
    //}
    return std::string(stream.str());
  }

  const bool DbWriter::isCompatGuard(const ConstrainedVariableId &var) const {
    std::set<ConstraintId> constrs;
    var->constraints(constrs);
    for(std::set<ConstraintId>::const_iterator it = constrs.begin();
	it != constrs.end(); ++it)
      if((*it)->getName() == LabelStr("RuleVariableListener"))
	return true;
    return false;
  }

  const std::string DbWriter::getVarInfo(const ConstrainedVariableId &varId) const {
    std::string type, paramName, predName, retval;
    if(varId->getIndex() >= I_STATE && varId->getIndex() <= I_END) {
      type = tokenVarTypes[varId->getIndex()];
    }
    else {
      type = PARAMETER_VAR;
      paramName = varId->getName().toString();
    }
    if(TokenId::convertable(varId->parent())) {
      predName = ((TokenId &)varId->parent())->getPredicateName().toString();
    }
    else if(ObjectId::convertable(varId->parent())) {
      predName = ((ObjectId &)varId->parent())->getName().toString();
    }
    else {
      predName = "UNKNOWN VARIABLE PARENT";
    }
    retval = type + COMMA + predName + COMMA + paramName + COMMA;
    return retval;
  }

  void DbWriter::addSourcePath(const char* path) {
    std::string spath(path);
    if((strcmp(path,"") == 0) || (strcmp(path,":") == 0))
      return;

    debugMsg("DbWriter:addSourcePath", "Adding path '" << spath << "'");
    sourcePaths.push_back(spath);
  }

  void DbWriter::write(TICK tick, unsigned int attempt) {

    /*
     * init output destination files if this has not been done
     * This is also called in WriteStatsAndTransactions() to cover
     * cases where the first step is not written.
     */

    // std::cerr << " DbWriter::write() called " << std::endl;
    check_error(!m_writing, "DbWriter attempted to write while writing.");
    m_writing = true;
    debugMsg("DbWriter:write", "Writing step " << stepCount);

    if(!destAlreadyInitialized) {
      initOutputDestination();
      destAlreadyInitialized = true;
    }
    if(!statsOut)
      return;
    ppId = 0LL;
    struct timeval currTime;
    if(gettimeofday(&currTime, NULL)) {
      FatalError("gettimeofday()", "Failed to get current time.");
    }
    ppId = timeval2Id(currTime);

    numTokens = numVariables = numConstraints = 0;

    // Generate step string
    std::ostringstream oss;
    oss<<tick<<"."<<attempt<<"."<<STEP;
    
    std::string stepnum = oss.str();

    std::string ppDest = dest + SLASH + stepnum;
    if(mkdir(ppDest.c_str(), 0777) && errno != EEXIST) {
      std::cerr << "Failed to create " << ppDest << std::endl;
      FatalErrno();
    }

    stepnum = "plan";

    std::string ppPartialPlan = ppDest + SLASH + stepnum + PARTIAL_PLAN;
    std::ofstream ppOut(ppPartialPlan.c_str());
    if(!ppOut) {
      FatalErrno();
    }

    ppOut << stepnum << TAB << ppId << TAB << pdbId->getSchema()->getName().toString()
	  << TAB << seqId << std::endl;
    ppOut.close();

    std::string ppObj = ppDest + SLASH + stepnum + OBJECTS;
    std::ofstream objOut(ppObj.c_str());
    if(!objOut) {
      FatalErrno();
    }

    std::string ppTok = ppDest + SLASH + stepnum + TOKENS;
    std::ofstream tokOut(ppTok.c_str());
    if(!tokOut) {
      FatalErrno();
    }

    std::string ppRuleInstances = ppDest + SLASH + stepnum + RULE_INSTANCES;
    std::ofstream ruleInstanceOut(ppRuleInstances.c_str());
    if(!ruleInstanceOut) {
      FatalErrno();
    }

    std::string ppRISM = ppDest + SLASH + stepnum + RULE_INSTANCE_SLAVE_MAP;
    std::ofstream rismOut(ppRISM.c_str());
    if(!rismOut) {
      FatalErrno();
    }

    std::string ppVars = ppDest + SLASH + stepnum + VARIABLES;
    std::ofstream varOut(ppVars.c_str());
    if(!varOut) {
      FatalErrno();
    }

    std::string ppConstrs = ppDest + SLASH + stepnum + CONSTRAINTS;
    std::ofstream constrOut(ppConstrs.c_str());
    if(!constrOut) {
      FatalErrno();
    }

    std::string ppCVM = ppDest + SLASH + stepnum + CONSTRAINT_VAR_MAP;
    std::ofstream cvmOut(ppCVM.c_str());
    if(!cvmOut) {
      FatalErrno();
    }

    std::string ppInsts = ppDest + SLASH + stepnum + INSTANTS;
    std::ofstream instsOut(ppInsts.c_str());
    if(!instsOut) {
      FatalErrno();
    }

    std::string ppDecs = ppDest + SLASH + stepnum + DECISIONS;
    std::ofstream decsOut(ppDecs.c_str());
    if(!decsOut) {
      FatalErrno();
    }

    const ConstraintSet &constraints = ceId->getConstraints();
    numConstraints = constraints.size();
    for(ConstraintSet::const_iterator it = constraints.begin(); it != constraints.end(); ++it) {
      outputConstraint(*it, constrOut, cvmOut);
    }

    ObjectSet objects(pdbId->getObjects());
    TokenSet tokens(pdbId->getTokens());
    int slotId = 1000000;
    for(ObjectSet::iterator objectIterator = objects.begin();
	objectIterator != objects.end(); ++objectIterator) {
      const ObjectId &objId = *objectIterator;
      if(TimelineId::convertable(objId)) {
	outputObject(objId, O_TIMELINE, objOut, varOut);
	TimelineId &tId = (TimelineId &) objId;
	const std::list<TokenId>& orderedTokens = tId->getTokenSequence();
	int slotIndex = 0;
	int emptySlots = 0;
	for(std::list<TokenId>::const_iterator tokenIterator = orderedTokens.begin();
	    tokenIterator != orderedTokens.end(); ++tokenIterator) {
	  int slotOrder = 0;
	  const TokenId &token = *tokenIterator;
	  outputToken(token, T_INTERVAL, slotId, slotIndex, slotOrder, (ObjectId) tId, tokOut,
		      varOut);
	  tokens.erase(token);
	  TokenSet::const_iterator mergedTokenIterator = 
	    token->getMergedTokens().begin();
	  for(;mergedTokenIterator != token->getMergedTokens().end(); ++mergedTokenIterator) {
	    slotOrder++;
	    outputToken(*mergedTokenIterator, T_INTERVAL, slotId, slotIndex, slotOrder,
			(ObjectId &) tId, tokOut, varOut);
	    tokens.erase(*mergedTokenIterator);
	  }
	  slotId++;
	  slotIndex++;
	  ++tokenIterator;
	  /*ExtraData: empty slot info*/
	  if(tokenIterator != orderedTokens.end()) {
	    const TokenId &nextToken = *tokenIterator;
	    if(token->end()->lastDomain() != nextToken->start()->lastDomain()) {
	      objOut << slotId << COMMA << slotIndex << COLON;
	      emptySlots++;
	      slotId++;
	      slotIndex++;
	    }
	  }
	  --tokenIterator;
	}
	if(!emptySlots)
	  objOut << SNULL;
	objOut << std::endl;
      }
#ifndef NO_RESOURCES
      /* TODO JRB: move this to resource module	  
	 else if(ResourceId::convertable(objId)) {
	 outputObject(objId, O_RESOURCE, objOut, varOut);

	 ResourceId &rId = (ResourceId &) objId;
          
	 //ExtraData: resource info
	 objOut << MINUS_INFINITY << COMMA << PLUS_INFINITY << COMMA
	 << rId->getInitialCapacity() << COMMA << rId->getLimitMin() << COMMA
	 << rId->getLimitMax() << COMMA;

	 //const std::set<TransactionId>& resTrans = rId->getTransactions();
	 std::set<TransactionId> resTrans;
	 rId->getTransactions(resTrans, MINUS_INFINITY, PLUS_INFINITY, false);
	 for(std::set<TransactionId>::iterator transIt = resTrans.begin();
	 transIt != resTrans.end(); ++transIt) {
	 TransactionId trans = *transIt;
	 outputToken(trans, T_TRANSACTION, 0, 1, 0, rId, tokOut, varOut);
	 tokens.erase(trans);
	 }

	 const std::map<int, InstantId>& insts = rId->getInstants();
	 for(std::map<int,InstantId>::const_iterator instIt = insts.begin();
	 instIt != insts.end(); ++instIt) {
	 InstantId inst = (*instIt).second;
	 outputInstant(inst, rId->getKey(), instsOut);
	 objOut << inst->getKey() << COMMA;
	 }
	 objOut << std::endl;
	 }
      */	  
#endif
      else {
	outputObject(objId, O_OBJECT, objOut, varOut);
	/*ExtraData: NULL*/
	objOut << SNULL << std::endl;
      }
    }
    for(TokenSet::iterator tokenIterator = tokens.begin(); 
	tokenIterator != tokens.end(); ++tokenIterator) {
      TokenId token = *tokenIterator;
      check_error(token.isValid());
      outputToken(token, T_INTERVAL, 0, 0, 0, ObjectId::noId(), tokOut, varOut);
    }



    std::set<RuleInstanceId> ruleInst = reId->getRuleInstances();
    for(std::set<RuleInstanceId>::const_iterator it = ruleInst.begin();
	it != ruleInst.end(); ++it) {
      RuleInstanceId ri = *it;
      outputRuleInstance(ri, ruleInstanceOut, varOut, rismOut);
    }
			
    collectStats(); // this call will overwrite incremental counters for tokens, variables, and constraints
    (*statsOut) << seqId << TAB << ppId << TAB << stepCount << TAB << numTokens << TAB << numVariables
		<< TAB << numConstraints << std::endl;
    statsOut->flush();

    objOut.close();
    tokOut.close();
    ruleInstanceOut.close();
    rismOut.close();
    varOut.close();
    constrOut.close();
    cvmOut.close();
    instsOut.close();
    decsOut.close();
    m_writing = false;
    stepCount++;
  }

  void DbWriter::initOutputDestination() {
    debugMsg("DbWriter:initOutputDestination", "Running...");

    char *destBuf = new char[PATH_MAX];
    if(realpath(dest.c_str(), destBuf) == NULL) {
      debugMsg("DbWriter:initOutputDestination", "Creating dest dir '" << destBuf << "'");
      if(mkdir(destBuf, 0777) && errno != EEXIST) {
	std::cerr << "Failed to make destination directory " << dest << std::endl;
	FatalErrno();
      }
    }
    realpath(dest.c_str(), destBuf);
    dest = destBuf;
    delete [] destBuf;

    char timestr[NBBY * sizeof(seqId) * 28/93 + 4];
    sprintf(timestr, "%lld", seqId);
    std::string modelName = pdbId->getSchema()->getName().toString();
    {
      std::string::size_type tempIndex = modelName.rfind('/');
      if(tempIndex > 0 && tempIndex < modelName.length()) {
	modelName = modelName.substr(tempIndex);
      }
    }

    //std::string seqName = m_agentName + "." + m_reactorName;
    //std::string::size_type extStart = seqName.find('.');
    //seqName = seqName.substr(0, extStart);

    if(mkdir(dest.c_str(), 0777) && errno != EEXIST) {
      /*
       * Failing with return code = -1, errno = 0 when dir already 
       * exists.  Should be returning errno = EEXIST. 
       * Check errno is non-zero before call to FatalErrno()
       */
      if (errno) {
	std::cerr << "Failed to make directory " << dest << std::endl;
	FatalErrno();
      }
    }
    //if(seqName[0] != '/')
    //  dest += "/";
    //dest += seqName;
    //dest += timestr;

    if(mkdir(dest.c_str(), 0777) && errno != EEXIST) {
      std::cerr << "Failed to make directory " << dest << std::endl;
      FatalErrno();
    }
    std::string ppStats(dest + PARTIAL_PLAN_STATS);
    std::string seqRules(dest + RULES);
    std::string seqStr(dest + SEQUENCE);
    std::ofstream seqOut(seqStr.c_str());
    if(!seqOut) {
      std::cerr << "Failed to open " << seqStr << std::endl;
      FatalErrno();
    }
    seqOut << dest << SEQ_COL_SEP << seqId << SEQ_COL_SEP;// << std::endl;
        
    std::ofstream rulesOut(seqRules.c_str());
    if(!rulesOut) {
      std::cerr << "Failed to open " << seqRules << std::endl;
      FatalErrno();
    }
      
    std::set<std::string> modelFiles;
    char realModelPaths[PATH_MAX];
    bool foundModelPath = false;
    const RuleSchemaId& rs = reId->getRuleSchema();

    for(std::multimap<double, RuleId>::const_iterator it = rs->getRules().begin(); 
	it != rs->getRules().end(); ++it) {
      std::string ruleSrc = ((*it).second)->getSource().toString();
      if(ruleSrc == "noSrc" || ruleSrc == "") {
	std::cerr << "Reject rule: " << (*it).second->getName().toString() << " : " << ruleSrc << std::endl;
	continue;
      }
      std::string modelFile = ruleSrc.substr(1, ruleSrc.rfind(",")-1);
      std::string lineNumber = ruleSrc.substr(ruleSrc.rfind(","), ruleSrc.size()-1);
      lineNumber.replace(lineNumber.rfind('"'), 1, "\0");
      foundModelPath = false;
      std::string tried = "";
      for(std::list<std::string>::iterator pathIt = sourcePaths.begin();
	  pathIt != sourcePaths.end(); ++pathIt) {
	std::string modelPath = (*pathIt) + "/" + modelFile;
	tried = tried + "\n" + modelPath;
	if(realpath(modelPath.c_str(), realModelPaths) == NULL) {
	  continue;
	}
	modelPath = realModelPaths;
	modelFiles.insert(modelPath);
	foundModelPath = true;
	rulesOut << seqId << TAB << (*it).second->getName().toString() << TAB << modelPath << lineNumber 
		 << std::endl;
	break;
      }
      if (foundModelPath == false) {
	std::cerr << "Warning: PPW could not find path to model file for rule " 
		  << (*it).second->getName().toString() << std::endl;
	std::cerr << "         Check configuration of RuleConfigSection in PlanWorks.cfg. Paths:" << tried << std::endl;
      }
    }
    {
      std::ostream_iterator<unsigned char> out(seqOut);
      for(std::set<std::string>::const_iterator it = modelFiles.begin(); 
	  it != modelFiles.end(); ++it) {
	seqOut << "--begin " << *it << std::endl;
	std::ifstream modelIn((*it).c_str());
	modelIn.unsetf(std::ios::skipws);
	std::copy(std::istream_iterator<unsigned char>(modelIn), 
		  std::istream_iterator<unsigned char>(), out);
      }
    }
    seqOut << SEQ_LINE_SEP;
    seqOut.close();

    statsOut = new std::ofstream(ppStats.c_str());
    if(!(*statsOut)) {
      FatalErrno();
    }
  }

}
