// GamePlay.nddl

#include "GamePlay.hh"
#include "NddlUtils.hh"

namespace NDDL {
  
  TokenStates TokenStatesBaseDomain(){
    static TokenStates sl_enum("TokenStates");
    if (sl_enum.isOpen()) {
      // Insert values to initialize
      sl_enum.insert(LabelStr("INACTIVE"));
      sl_enum.insert(LabelStr("ACTIVE"));
      sl_enum.insert(LabelStr("MERGED"));
      sl_enum.insert(LabelStr("REJECTED"));
      sl_enum.close();
    }
    return(sl_enum);
  }
  // SKIPPING IMPLEMENTATION FOR BUILT-IN CLASS Timeline
  
  
  
  // StringData.nddl:5 StringData
  StringData::StringData(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : Object(planDatabase, "StringData", name, true) {
  }
  StringData::StringData(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : Object(planDatabase, type, name, true) {
  }
  StringData::StringData(const ObjectId& parent, const LabelStr& name)
   : Object(parent, "StringData", name, true) {}
  StringData::StringData(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : Object(parent, type, name, true) {}
  // default initialization of member variables
  void StringData::handleDefaults(bool autoClose) {
    if(data.isNoId()){
      data = addVariable(StringDomain("string"), "data");
    }
    if (autoClose)
      close();
  }
  
  
  // StringData.nddl:5 StringData
  void StringData::constructor(const LabelStr& _data) {
    data = addVariable(StringDomain(_data, "string"), "data");
  }
  
  // StringData.nddl:5 StringData
  class StringDataFactory0: public ObjectFactory {
  public:
    StringDataFactory0(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("string")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to string");
      check_error(arguments[0]->isSingleton());
      LabelStr _data((LabelStr)arguments[0]->getSingletonValue());
      
      StringDataId instance = (new StringData(planDb, objectType, objectName))->getId();
      instance->constructor(_data);
      instance->handleDefaults();
      return instance;
    }
  };
  
  Mode ModeBaseDomain(){
    static Mode sl_enum("Mode");
    if (sl_enum.isOpen()) {
      // Insert values to initialize
      sl_enum.insert(LabelStr("External"));
      sl_enum.insert(LabelStr("Internal"));
      sl_enum.insert(LabelStr("Ignore"));
      sl_enum.close();
    }
    return(sl_enum);
  }
  
  
  // TREX.nddl:15 AgentTimeline
  AgentTimeline::AgentTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : Timeline(planDatabase, "AgentTimeline", name, true) {
  }
  AgentTimeline::AgentTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : Timeline(planDatabase, type, name, true) {
  }
  AgentTimeline::AgentTimeline(const ObjectId& parent, const LabelStr& name)
   : Timeline(parent, "AgentTimeline", name, true) {}
  AgentTimeline::AgentTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : Timeline(parent, type, name, true) {}
  // default initialization of member variables
  void AgentTimeline::handleDefaults(bool autoClose) {
    if(mode.isNoId()){
      mode = addVariable(ModeBaseDomain(), "mode");
    }
    if(defaultPredicate.isNoId()){
      defaultPredicate = addVariable(StringDomain("string"), "defaultPredicate");
    }
    if (autoClose)
      close();
  }
  
  
  
  // TREX.nddl:23 undefined
  AgentTimeline::undefined::undefined(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  AgentTimeline::undefined::undefined(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void AgentTimeline::undefined::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // TREX.nddl:28 Terminated
  AgentTimeline::Terminated::Terminated(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  AgentTimeline::Terminated::Terminated(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void AgentTimeline::Terminated::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
    
    // Post parameter constraints
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("end")));
      vars.push_back(var(getId(),std::string("missionEnd")));
      token_constraint(concurrent, vars);
    }
  }
  
  
  // TREX.nddl:15 AgentTimeline
  void AgentTimeline::constructor(const LabelStr& _mode) {
    mode = addVariable(Mode(_mode, "Mode"), "mode");
    defaultPredicate = addVariable(StringDomain(LabelStr("undefined"), "string"), "defaultPredicate");
  }
  
  // TREX.nddl:15 AgentTimeline
  class AgentTimelineFactory1: public ObjectFactory {
  public:
    AgentTimelineFactory1(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      AgentTimelineId instance = (new AgentTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // TREX.nddl:15 AgentTimeline
  void AgentTimeline::constructor(const LabelStr& _mode, const LabelStr& _defaultPredicate) {
    mode = addVariable(Mode(_mode, "Mode"), "mode");
    defaultPredicate = addVariable(StringDomain(_defaultPredicate, "string"), "defaultPredicate");
  }
  
  // TREX.nddl:15 AgentTimeline
  class AgentTimelineFactory2: public ObjectFactory {
  public:
    AgentTimelineFactory2(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 2);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[1], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("string")), 
                  "Cannot convert " + arguments[1]->getTypeName().toString() + " to string");
      check_error(arguments[1]->isSingleton());
      LabelStr _defaultPredicate((LabelStr)arguments[1]->getSingletonValue());
      
      AgentTimelineId instance = (new AgentTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode, _defaultPredicate);
      instance->handleDefaults();
      return instance;
    }
  };
  
  
  // TREX.nddl:45 AgentActions
  AgentActions::AgentActions(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : Object(planDatabase, "AgentActions", name, true) {
  }
  AgentActions::AgentActions(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : Object(planDatabase, type, name, true) {
  }
  AgentActions::AgentActions(const ObjectId& parent, const LabelStr& name)
   : Object(parent, "AgentActions", name, true) {}
  AgentActions::AgentActions(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : Object(parent, type, name, true) {}
  // default initialization of member variables
  void AgentActions::handleDefaults(bool autoClose) {
    if (autoClose)
      close();
  }
  
  
  // TREX.nddl:45 AgentActions
  void AgentActions::constructor() {
  }
  
  // TREX.nddl:45 AgentActions
  class AgentActionsFactory3: public ObjectFactory {
  public:
    AgentActionsFactory3(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 0);
      AgentActionsId instance = (new AgentActions(planDb, objectType, objectName))->getId();
      instance->constructor();
      instance->handleDefaults();
      return instance;
    }
  };
  
  
  // TREX.nddl:49 StateVariable
  StateVariable::StateVariable(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "StateVariable", name) {
  }
  StateVariable::StateVariable(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  StateVariable::StateVariable(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "StateVariable", name) {}
  StateVariable::StateVariable(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void StateVariable::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // TREX.nddl:50 Holds
  StateVariable::Holds::Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  StateVariable::Holds::Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void StateVariable::Holds::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
    
    // Post parameter constraints
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(predicateVariable(IntervalIntDomain(1,1, "int")));
      token_constraint(eq, vars);
    }
  }
  
  
  // TREX.nddl:49 StateVariable
  void StateVariable::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // TREX.nddl:49 StateVariable
  class StateVariableFactory4: public ObjectFactory {
  public:
    StateVariableFactory4(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      StateVariableId instance = (new StateVariable(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // TREX.nddl:49 StateVariable
  void StateVariable::constructor(const LabelStr& _mode, const LabelStr& _defaultPredicate) {
    AgentTimeline::constructor(_mode, _defaultPredicate);
  }
  
  // TREX.nddl:49 StateVariable
  class StateVariableFactory5: public ObjectFactory {
  public:
    StateVariableFactory5(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 2);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[1], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("string")), 
                  "Cannot convert " + arguments[1]->getTypeName().toString() + " to string");
      check_error(arguments[1]->isSingleton());
      LabelStr _defaultPredicate((LabelStr)arguments[1]->getSingletonValue());
      
      StateVariableId instance = (new StateVariable(planDb, objectType, objectName))->getId();
      instance->constructor(_mode, _defaultPredicate);
      instance->handleDefaults();
      return instance;
    }
  };
  
  
  // dispatchlib.nddl:4 CreatorTimeline
  CreatorTimeline::CreatorTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "CreatorTimeline", name) {
  }
  CreatorTimeline::CreatorTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  CreatorTimeline::CreatorTimeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "CreatorTimeline", name) {}
  CreatorTimeline::CreatorTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void CreatorTimeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // dispatchlib.nddl:5 Gamma
  CreatorTimeline::Gamma::Gamma(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  CreatorTimeline::Gamma::Gamma(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void CreatorTimeline::Gamma::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // dispatchlib.nddl:6 Delta
  CreatorTimeline::Delta::Delta(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  CreatorTimeline::Delta::Delta(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void CreatorTimeline::Delta::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  // dispatchlib.nddl:4 CreatorTimeline
  void CreatorTimeline::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // dispatchlib.nddl:4 CreatorTimeline
  class CreatorTimelineFactory6: public ObjectFactory {
  public:
    CreatorTimelineFactory6(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      CreatorTimelineId instance = (new CreatorTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // dispatchlib.nddl:13 Gamma
  class CreatorTimeline$Gamma$0$0: public RuleInstance {
  public:
    CreatorTimeline$Gamma$0$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$CreatorTimeline$Gamma$0, CreatorTimeline$Gamma$0$0, CreatorTimeline.Gamma, "dispatchlib.nddl,13");
  
  void CreatorTimeline$Gamma$0$0::handleExecute() {
    localSlave(Delta, slave0, "meets");
    sameObject(object, slave0);
    meets(this, slave0);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(ruleVariable(IntervalIntDomain(3,3, "int")));
      rule_constraint(leq, vars);
    }
  }
  
  
  // dispatchlib.nddl:18 ReciverTimeline
  ReciverTimeline::ReciverTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "ReciverTimeline", name) {
  }
  ReciverTimeline::ReciverTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  ReciverTimeline::ReciverTimeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "ReciverTimeline", name) {}
  ReciverTimeline::ReciverTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void ReciverTimeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // dispatchlib.nddl:19 Alpha
  ReciverTimeline::Alpha::Alpha(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  ReciverTimeline::Alpha::Alpha(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void ReciverTimeline::Alpha::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // dispatchlib.nddl:20 Beta
  ReciverTimeline::Beta::Beta(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  ReciverTimeline::Beta::Beta(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void ReciverTimeline::Beta::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  // dispatchlib.nddl:18 ReciverTimeline
  void ReciverTimeline::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // dispatchlib.nddl:18 ReciverTimeline
  class ReciverTimelineFactory7: public ObjectFactory {
  public:
    ReciverTimelineFactory7(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      ReciverTimelineId instance = (new ReciverTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // dispatchlib.nddl:27 Beta
  class ReciverTimeline$Beta$1$0: public RuleInstance {
  public:
    ReciverTimeline$Beta$1$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$ReciverTimeline$Beta$1, ReciverTimeline$Beta$1$0, ReciverTimeline.Beta, "dispatchlib.nddl,27");
  
  void ReciverTimeline$Beta$1$0::handleExecute() {
    slave(CreatorTimeline::Gamma, CreatorTimeline.Gamma, slave1, LabelStr("after"));
    after(this, slave1);
  }
  
  
  // dispatchlib.nddl:31 EqReciverTimeline
  EqReciverTimeline::EqReciverTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "EqReciverTimeline", name) {
  }
  EqReciverTimeline::EqReciverTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  EqReciverTimeline::EqReciverTimeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "EqReciverTimeline", name) {}
  EqReciverTimeline::EqReciverTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void EqReciverTimeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // dispatchlib.nddl:32 Alpha
  EqReciverTimeline::Alpha::Alpha(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  EqReciverTimeline::Alpha::Alpha(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void EqReciverTimeline::Alpha::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // dispatchlib.nddl:33 Beta
  EqReciverTimeline::Beta::Beta(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  EqReciverTimeline::Beta::Beta(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void EqReciverTimeline::Beta::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  // dispatchlib.nddl:31 EqReciverTimeline
  void EqReciverTimeline::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // dispatchlib.nddl:31 EqReciverTimeline
  class EqReciverTimelineFactory8: public ObjectFactory {
  public:
    EqReciverTimelineFactory8(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      EqReciverTimelineId instance = (new EqReciverTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // dispatchlib.nddl:40 Beta
  class EqReciverTimeline$Beta$2$0: public RuleInstance {
  public:
    EqReciverTimeline$Beta$2$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$EqReciverTimeline$Beta$2, EqReciverTimeline$Beta$2$0, EqReciverTimeline.Beta, "dispatchlib.nddl,40");
  
  void EqReciverTimeline$Beta$2$0::handleExecute() {
    slave(CreatorTimeline::Gamma, CreatorTimeline.Gamma, slave2, LabelStr("met_by"));
    met_by(this, slave2);
  }
  
  Values ValuesBaseDomain(){
    static Values sl_enum("Values");
    if (sl_enum.isOpen()) {
      // Insert values to initialize
      sl_enum.insert(LabelStr("Rock"));
      sl_enum.insert(LabelStr("Paper"));
      sl_enum.insert(LabelStr("Scissors"));
      sl_enum.close();
    }
    return(sl_enum);
  }
  
  
  // GamePlay.nddl:6 PlayerState
  PlayerState::PlayerState(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "PlayerState", name) {
  }
  PlayerState::PlayerState(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  PlayerState::PlayerState(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "PlayerState", name) {}
  PlayerState::PlayerState(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void PlayerState::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:7 Hides
  PlayerState::Hides::Hides(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  PlayerState::Hides::Hides(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void PlayerState::Hides::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:8 Shows
  PlayerState::Shows::Shows(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  PlayerState::Shows::Shows(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void PlayerState::Shows::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(value.isNoId()){
      value = addParameter(ValuesBaseDomain(), "value");
    }
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:6 PlayerState
  void PlayerState::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // GamePlay.nddl:6 PlayerState
  class PlayerStateFactory9: public ObjectFactory {
  public:
    PlayerStateFactory9(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      PlayerStateId instance = (new PlayerState(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:15 Hides
  class PlayerState$Hides$3$0: public RuleInstance {
  public:
    PlayerState$Hides$3$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$PlayerState$Hides$3, PlayerState$Hides$3$0, PlayerState.Hides, "GamePlay.nddl,15");
  
  void PlayerState$Hides$3$0::handleExecute() {
    localSlave(Shows, slave3, "meets");
    sameObject(object, slave3);
    meets(this, slave3);
    localSlave(Shows, slave4, "met_by");
    sameObject(object, slave4);
    met_by(this, slave4);
  }
  
  // GamePlay.nddl:20 Shows
  class PlayerState$Shows$4$0: public RuleInstance {
  public:
    PlayerState$Shows$4$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$PlayerState$Shows$4, PlayerState$Shows$4$0, PlayerState.Shows, "GamePlay.nddl,20");
  
  void PlayerState$Shows$4$0::handleExecute() {
    localSlave(Hides, slave5, "meets");
    sameObject(object, slave5);
    meets(this, slave5);
    localSlave(Hides, slave6, "met_by");
    sameObject(object, slave6);
    met_by(this, slave6);
  }
  
  
  // GamePlay.nddl:25 Player
  Player::Player(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "Player", name) {
  }
  Player::Player(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  Player::Player(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "Player", name) {}
  Player::Player(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void Player::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    check_error(hand.isValid(), "object variables must be initialized explicitly");
    
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:27 Wait
  Player::Wait::Wait(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  Player::Wait::Wait(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void Player::Wait::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:28 One
  Player::One::One(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
    getVariable("duration")->restrictBaseDomain(IntervalIntDomain(1, 1, "int"));
  }
  
  Player::One::One(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
    getVariable("duration")->restrictBaseDomain(IntervalIntDomain(1, 1, "int"));
  }
  
  // default initialization of member variables
  void Player::One::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:29 Two
  Player::Two::Two(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
    getVariable("duration")->restrictBaseDomain(IntervalIntDomain(1, 1, "int"));
  }
  
  Player::Two::Two(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
    getVariable("duration")->restrictBaseDomain(IntervalIntDomain(1, 1, "int"));
  }
  
  // default initialization of member variables
  void Player::Two::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:30 Three
  Player::Three::Three(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
    getVariable("duration")->restrictBaseDomain(IntervalIntDomain(1, 1, "int"));
  }
  
  Player::Three::Three(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
    getVariable("duration")->restrictBaseDomain(IntervalIntDomain(1, 1, "int"));
  }
  
  // default initialization of member variables
  void Player::Three::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:31 Draw
  Player::Draw::Draw(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
    getVariable("duration")->restrictBaseDomain(IntervalIntDomain(1, 1, "int"));
  }
  
  Player::Draw::Draw(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
    getVariable("duration")->restrictBaseDomain(IntervalIntDomain(1, 1, "int"));
  }
  
  // default initialization of member variables
  void Player::Draw::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:25 Player
  void Player::constructor(const LabelStr& _playerMode, const LabelStr& _stateMode) {
    AgentTimeline::constructor(_playerMode);
    hand = addVariable(PlayerStateDomain((new PlayerState(m_id, "hand"))->getId(), "PlayerState"), "hand");
    Id<PlayerState>(singleton(hand))->constructor(_stateMode);
    Id<PlayerState>(singleton(hand))->handleDefaults();
  }
  
  // GamePlay.nddl:25 Player
  class PlayerFactory10: public ObjectFactory {
  public:
    PlayerFactory10(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 2);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _playerMode((LabelStr)arguments[0]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[1], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[1]->getTypeName().toString() + " to Mode");
      check_error(arguments[1]->isSingleton());
      LabelStr _stateMode((LabelStr)arguments[1]->getSingletonValue());
      
      PlayerId instance = (new Player(planDb, objectType, objectName))->getId();
      instance->constructor(_playerMode, _stateMode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  
  // GamePlay.nddl:38 Umpire
  Umpire::Umpire(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "Umpire", name) {
  }
  Umpire::Umpire(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  Umpire::Umpire(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "Umpire", name) {}
  Umpire::Umpire(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void Umpire::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    check_error(player1.isValid(), "object variables must be initialized explicitly");
    
    check_error(player2.isValid(), "object variables must be initialized explicitly");
    
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:42 PlayGame
  Umpire::PlayGame::PlayGame(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
    getVariable("duration")->restrictBaseDomain(IntervalIntDomain(4, 4, "int"));
  }
  
  Umpire::PlayGame::PlayGame(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
    getVariable("duration")->restrictBaseDomain(IntervalIntDomain(4, 4, "int"));
  }
  
  // default initialization of member variables
  void Umpire::PlayGame::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:43 Wait
  Umpire::Wait::Wait(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  Umpire::Wait::Wait(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void Umpire::Wait::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:38 Umpire
  void Umpire::constructor(const PlayerId& _player1, const PlayerId& _player2, const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
    player1 = addVariable(PlayerDomain(_player1, "Player"), "player1");
    player2 = addVariable(PlayerDomain(_player2, "Player"), "player2");
  }
  
  // GamePlay.nddl:38 Umpire
  class UmpireFactory11: public ObjectFactory {
  public:
    UmpireFactory11(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 3);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Player")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Player");
      check_error(arguments[0]->isSingleton());
      PlayerId _player1((PlayerId)arguments[0]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[1], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Player")), 
                  "Cannot convert " + arguments[1]->getTypeName().toString() + " to Player");
      check_error(arguments[1]->isSingleton());
      PlayerId _player2((PlayerId)arguments[1]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[2], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[2]->getTypeName().toString() + " to Mode");
      check_error(arguments[2]->isSingleton());
      LabelStr _mode((LabelStr)arguments[2]->getSingletonValue());
      
      UmpireId instance = (new Umpire(planDb, objectType, objectName))->getId();
      instance->constructor(_player1, _player2, _mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:52 Wait
  class Player$Wait$5$0: public RuleInstance {
  public:
    Player$Wait$5$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$Player$Wait$5, Player$Wait$5$0, Player.Wait, "GamePlay.nddl,52");
  
  void Player$Wait$5$0::handleExecute() {
    localSlave(Draw, slave7, "met_by");
    sameObject(object, slave7);
    met_by(this, slave7);
    localSlave(One, slave8, "meets");
    sameObject(object, slave8);
    meets(this, slave8);
    slave(PlayerState::Hides, PlayerState.Hides, slave9, LabelStr("contained_by"));
    constrainObject(object ,hand,slave9);
    contained_by(this, slave9);
  }
  
  // GamePlay.nddl:58 One
  class Player$One$6$0: public RuleInstance {
  public:
    Player$One$6$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$Player$One$6, Player$One$6$0, Player.One, "GamePlay.nddl,58");
  
  void Player$One$6$0::handleExecute() {
    localSlave(Wait, slave10, "met_by");
    sameObject(object, slave10);
    met_by(this, slave10);
    localSlave(Two, slave11, "meets");
    sameObject(object, slave11);
    meets(this, slave11);
    slave(PlayerState::Hides, PlayerState.Hides, slave12, LabelStr("contained_by"));
    constrainObject(object ,hand,slave12);
    contained_by(this, slave12);
  }
  
  // GamePlay.nddl:64 Two
  class Player$Two$7$0: public RuleInstance {
  public:
    Player$Two$7$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$Player$Two$7, Player$Two$7$0, Player.Two, "GamePlay.nddl,64");
  
  void Player$Two$7$0::handleExecute() {
    localSlave(One, slave13, "met_by");
    sameObject(object, slave13);
    met_by(this, slave13);
    localSlave(Three, slave14, "meets");
    sameObject(object, slave14);
    meets(this, slave14);
    slave(PlayerState::Hides, PlayerState.Hides, slave15, LabelStr("contained_by"));
    constrainObject(object ,hand,slave15);
    contained_by(this, slave15);
  }
  
  // GamePlay.nddl:70 Three
  class Player$Three$8$0: public RuleInstance {
  public:
    Player$Three$8$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$Player$Three$8, Player$Three$8$0, Player.Three, "GamePlay.nddl,70");
  
  void Player$Three$8$0::handleExecute() {
    localSlave(Two, slave16, "met_by");
    sameObject(object, slave16);
    met_by(this, slave16);
    localSlave(Draw, slave17, "meets");
    sameObject(object, slave17);
    meets(this, slave17);
    slave(PlayerState::Hides, PlayerState.Hides, slave18, LabelStr("contained_by"));
    constrainObject(object ,hand,slave18);
    contained_by(this, slave18);
  }
  
  // GamePlay.nddl:76 Draw
  class Player$Draw$9$0: public RuleInstance {
  public:
    Player$Draw$9$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$Player$Draw$9, Player$Draw$9$0, Player.Draw, "GamePlay.nddl,76");
  
  void Player$Draw$9$0::handleExecute() {
    localSlave(Three, slave19, "met_by");
    sameObject(object, slave19);
    met_by(this, slave19);
    localSlave(Wait, slave20, "meets");
    sameObject(object, slave20);
    meets(this, slave20);
    slave(PlayerState::Shows, PlayerState.Shows, slave21, LabelStr("contained_by"));
    constrainObject(object ,hand,slave21);
    contained_by(this, slave21);
  }
  
  // GamePlay.nddl:82 PlayGame
  class Umpire$PlayGame$10$0: public RuleInstance {
  public:
    Umpire$PlayGame$10$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$Umpire$PlayGame$10, Umpire$PlayGame$10$0, Umpire.PlayGame, "GamePlay.nddl,82");
  
  void Umpire$PlayGame$10$0::handleExecute() {
    localSlave(Wait, slave22, "met_by");
    sameObject(object, slave22);
    met_by(this, slave22);
    localSlave(Wait, slave23, "meets");
    sameObject(object, slave23);
    meets(this, slave23);
    slave(Player::Draw, Player.Draw, slave24, LabelStr("ends"));
    constrainObject(object ,player1,slave24);
    ends(this, slave24);
    slave(Player::Draw, Player.Draw, slave25, LabelStr("ends"));
    constrainObject(object ,player2,slave25);
    ends(this, slave25);
  }
  
  // GamePlay.nddl:89 Wait
  class Umpire$Wait$11$0: public RuleInstance {
  public:
    Umpire$Wait$11$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$Umpire$Wait$11, Umpire$Wait$11$0, Umpire.Wait, "GamePlay.nddl,89");
  
  void Umpire$Wait$11$0::handleExecute() {
    localSlave(PlayGame, slave26, "met_by");
    sameObject(object, slave26);
    met_by(this, slave26);
    localSlave(PlayGame, slave27, "meets");
    sameObject(object, slave27);
    meets(this, slave27);
    slave(Player::Wait, Player.Wait, slave28, LabelStr("equals"));
    constrainObject(object ,player1,slave28);
    equals(this, slave28);
    slave(Player::Wait, Player.Wait, slave29, LabelStr("equals"));
    constrainObject(object ,player2,slave29);
    equals(this, slave29);
  }
  
  
  // GamePlay.nddl:101 NumberTimeline
  NumberTimeline::NumberTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "NumberTimeline", name) {
  }
  NumberTimeline::NumberTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  NumberTimeline::NumberTimeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "NumberTimeline", name) {}
  NumberTimeline::NumberTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void NumberTimeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:102 holds
  NumberTimeline::holds::holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  NumberTimeline::holds::holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void NumberTimeline::holds::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(value.isNoId()){
      value = addParameter(IntervalIntDomain(0, 4, "int"), "value");
    }
    if (autoClose)
      close();
    
    // Post parameter constraints
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(predicateVariable(IntervalIntDomain(1,1, "int")));
      token_constraint(eq, vars);
    }
  }
  
  
  // GamePlay.nddl:101 NumberTimeline
  void NumberTimeline::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // GamePlay.nddl:101 NumberTimeline
  class NumberTimelineFactory12: public ObjectFactory {
  public:
    NumberTimelineFactory12(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      NumberTimelineId instance = (new NumberTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:112 holds
  class NumberTimeline$holds$12$0: public RuleInstance {
  public:
    NumberTimeline$holds$12$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$NumberTimeline$holds$12, NumberTimeline$holds$12$0, NumberTimeline.holds, "GamePlay.nddl,112");
  
  void NumberTimeline$holds$12$0::handleExecute() {
    localSlave(holds, slave30, "met_by");
    sameObject(object, slave30);
    met_by(this, slave30);
    localSlave(holds, slave31, "meets");
    sameObject(object, slave31);
    meets(this, slave31);
  }
  
  
  // GamePlay.nddl:117 A_Timeline
  A_Timeline::A_Timeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "A_Timeline", name) {
  }
  A_Timeline::A_Timeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  A_Timeline::A_Timeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "A_Timeline", name) {}
  A_Timeline::A_Timeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void A_Timeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:118 pred
  A_Timeline::pred::pred(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  A_Timeline::pred::pred(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void A_Timeline::pred::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:117 A_Timeline
  void A_Timeline::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // GamePlay.nddl:117 A_Timeline
  class A_TimelineFactory13: public ObjectFactory {
  public:
    A_TimelineFactory13(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      A_TimelineId instance = (new A_Timeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  
  // GamePlay.nddl:122 B_Timeline
  B_Timeline::B_Timeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "B_Timeline", name) {
  }
  B_Timeline::B_Timeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  B_Timeline::B_Timeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "B_Timeline", name) {}
  B_Timeline::B_Timeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void B_Timeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:123 pred1
  B_Timeline::pred1::pred1(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  B_Timeline::pred1::pred1(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void B_Timeline::pred1::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:124 pred2
  B_Timeline::pred2::pred2(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  B_Timeline::pred2::pred2(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void B_Timeline::pred2::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:122 B_Timeline
  void B_Timeline::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // GamePlay.nddl:122 B_Timeline
  class B_TimelineFactory14: public ObjectFactory {
  public:
    B_TimelineFactory14(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      B_TimelineId instance = (new B_Timeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:132 pred
  class A_Timeline$pred$13$0: public RuleInstance {
  public:
    A_Timeline$pred$13$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$A_Timeline$pred$13, A_Timeline$pred$13$0, A_Timeline.pred, "GamePlay.nddl,132");
  
  void A_Timeline$pred$13$0::handleExecute() {
    slave(B_Timeline::pred1, B_Timeline.pred1, a, LabelStr("starts_before"));
    starts_before(this, a);
    slave(B_Timeline::pred1, B_Timeline.pred1, b, LabelStr("starts_before"));
    starts_before(this, b);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("a")), std::string("object")));
      vars.push_back(varfromtok(tok(getId(), std::string("b")), std::string("object")));
      rule_constraint(neq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("a")), std::string("end")));
      vars.push_back(var(getId(),std::string("end")));
      rule_constraint(concurrent, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("a")), std::string("start")));
      vars.push_back(varfromtok(tok(getId(), std::string("b")), std::string("start")));
      rule_constraint(concurrent, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("a")), std::string("end")));
      vars.push_back(varfromtok(tok(getId(), std::string("b")), std::string("end")));
      rule_constraint(concurrent, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("a")), std::string("duration")));
      vars.push_back(varfromtok(tok(getId(), std::string("b")), std::string("duration")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("a")), std::string("duration")));
      vars.push_back(ruleVariable(IntervalIntDomain(4,4, "int")));
      rule_constraint(eq, vars);
    }
  }
  
  // GamePlay.nddl:149 pred1
  class B_Timeline$pred1$14$0: public RuleInstance {
  public:
    B_Timeline$pred1$14$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$B_Timeline$pred1$14, B_Timeline$pred1$14$0, B_Timeline.pred1, "GamePlay.nddl,149");
  
  void B_Timeline$pred1$14$0::handleExecute() {
    slave(B_Timeline::pred2, B_Timeline.pred2, p, LabelStr("equals"));
    equals(this, p);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("object")));
      vars.push_back(varfromtok(tok(getId(), std::string("p")), std::string("object")));
      rule_constraint(neq, vars);
    }
  }
  
  // GamePlay.nddl:154 pred2
  class B_Timeline$pred2$15$0: public RuleInstance {
  public:
    B_Timeline$pred2$15$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$B_Timeline$pred2$15, B_Timeline$pred2$15$0, B_Timeline.pred2, "GamePlay.nddl,154");
  
  void B_Timeline$pred2$15$0::handleExecute() {
    slave(B_Timeline::pred1, B_Timeline.pred1, p, LabelStr("equals"));
    equals(this, p);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("object")));
      vars.push_back(varfromtok(tok(getId(), std::string("p")), std::string("object")));
      rule_constraint(neq, vars);
    }
  }
  
  
  // GamePlay.nddl:163 CyclicTimeline
  CyclicTimeline::CyclicTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "CyclicTimeline", name) {
  }
  CyclicTimeline::CyclicTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  CyclicTimeline::CyclicTimeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "CyclicTimeline", name) {}
  CyclicTimeline::CyclicTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void CyclicTimeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:165 pred
  CyclicTimeline::pred::pred(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  CyclicTimeline::pred::pred(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void CyclicTimeline::pred::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:163 CyclicTimeline
  void CyclicTimeline::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // GamePlay.nddl:163 CyclicTimeline
  class CyclicTimelineFactory15: public ObjectFactory {
  public:
    CyclicTimelineFactory15(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      CyclicTimelineId instance = (new CyclicTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:172 pred
  class CyclicTimeline$pred$16$0: public RuleInstance {
  public:
    CyclicTimeline$pred$16$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$CyclicTimeline$pred$16, CyclicTimeline$pred$16$0, CyclicTimeline.pred, "GamePlay.nddl,172");
  
  void CyclicTimeline$pred$16$0::handleExecute() {
    localSlave(pred, t0, "any");
    sameObject(object, t0);
    localSlave(pred, t1, "any");
    sameObject(object, t1);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(ruleVariable(IntervalIntDomain(1,1, "int")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("t0")), std::string("end")));
      vars.push_back(ruleVariable(IntervalIntDomain(1,1, "int")));
      vars.push_back(var(getId(),std::string("start")));
      rule_constraint(temporalDistance, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("end")));
      vars.push_back(ruleVariable(IntervalIntDomain(1,1, "int")));
      vars.push_back(varfromtok(tok(getId(), std::string("t1")), std::string("start")));
      rule_constraint(temporalDistance, vars);
    }
  }
  
  
  // GamePlay.nddl:185 SensorTimeline
  SensorTimeline::SensorTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "SensorTimeline", name) {
  }
  SensorTimeline::SensorTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  SensorTimeline::SensorTimeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "SensorTimeline", name) {}
  SensorTimeline::SensorTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void SensorTimeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:186 holds
  SensorTimeline::holds::holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  SensorTimeline::holds::holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void SensorTimeline::holds::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(value.isNoId()){
      value = addParameter(IntervalIntDomain("int"), "value");
    }
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:185 SensorTimeline
  void SensorTimeline::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // GamePlay.nddl:185 SensorTimeline
  class SensorTimelineFactory16: public ObjectFactory {
  public:
    SensorTimelineFactory16(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      SensorTimelineId instance = (new SensorTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  
  // GamePlay.nddl:193 DerivedTimeline
  DerivedTimeline::DerivedTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "DerivedTimeline", name) {
  }
  DerivedTimeline::DerivedTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  DerivedTimeline::DerivedTimeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "DerivedTimeline", name) {}
  DerivedTimeline::DerivedTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void DerivedTimeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:194 holds
  DerivedTimeline::holds::holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  DerivedTimeline::holds::holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void DerivedTimeline::holds::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(value.isNoId()){
      value = addParameter(IntervalIntDomain("int"), "value");
    }
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:193 DerivedTimeline
  void DerivedTimeline::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // GamePlay.nddl:193 DerivedTimeline
  class DerivedTimelineFactory17: public ObjectFactory {
  public:
    DerivedTimelineFactory17(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      DerivedTimelineId instance = (new DerivedTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:201 holds
  class SensorTimeline$holds$17$0: public RuleInstance {
  public:
    SensorTimeline$holds$17$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$SensorTimeline$holds$17, SensorTimeline$holds$17$0, SensorTimeline.holds, "GamePlay.nddl,201");
  
  void SensorTimeline$holds$17$0::handleExecute() {
    slave(DerivedTimeline::holds, DerivedTimeline.holds, t, LabelStr("equals"));
    equals(this, t);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("value")));
      vars.push_back(ruleVariable(IntervalIntDomain(1,1, "int")));
      vars.push_back(varfromtok(tok(getId(), std::string("t")), std::string("value")));
      rule_constraint(addEq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(ruleVariable(IntervalIntDomain(1,1, "int")));
      rule_constraint(eq, vars);
    }
  }
  
  
  // GamePlay.nddl:211 Counter
  Counter::Counter(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "Counter", name) {
  }
  Counter::Counter(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  Counter::Counter(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "Counter", name) {}
  Counter::Counter(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void Counter::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:212 Holds
  Counter::Holds::Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  Counter::Holds::Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void Counter::Holds::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(value.isNoId()){
      value = addParameter(IntervalIntDomain("int"), "value");
    }
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:211 Counter
  void Counter::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode, "undefined");
  }
  
  // GamePlay.nddl:211 Counter
  class CounterFactory18: public ObjectFactory {
  public:
    CounterFactory18(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      CounterId instance = (new Counter(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:217 Holds
  class Counter$Holds$18$0: public RuleInstance {
  public:
    Counter$Holds$18$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$Counter$Holds$18, Counter$Holds$18$0, Counter.Holds, "GamePlay.nddl,217");
  
  void Counter$Holds$18$0::handleExecute() {
    localSlave(Holds, slave39, "meets");
    sameObject(object, slave39);
    meets(this, slave39);
  }
  
  
  // GamePlay.nddl:221 TestActions
  TestActions::TestActions(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentActions(planDatabase, "TestActions", name) {
  }
  TestActions::TestActions(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentActions(planDatabase, type, name) {
  }
  TestActions::TestActions(const ObjectId& parent, const LabelStr& name)
   : AgentActions(parent, "TestActions", name) {}
  TestActions::TestActions(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentActions(parent, type, name) {}
  // default initialization of member variables
  void TestActions::handleDefaults(bool autoClose) {
    AgentActions::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:223 increment
  TestActions::increment::increment(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  TestActions::increment::increment(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void TestActions::increment::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:225 multiply
  TestActions::multiply::multiply(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  TestActions::multiply::multiply(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void TestActions::multiply::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(multiplier.isNoId()){
      multiplier = addParameter(IntervalIntDomain("int"), "multiplier");
    }
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:227 iterator
  TestActions::iterator::iterator(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  TestActions::iterator::iterator(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void TestActions::iterator::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(counter.isNoId()){
      counter = addParameter(IntervalIntDomain("int"), "counter");
    }
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:229 doGrasp
  TestActions::doGrasp::doGrasp(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  TestActions::doGrasp::doGrasp(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void TestActions::doGrasp::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(x.isNoId()){
      x = addParameter(IntervalIntDomain("int"), "x");
    }
    if(y.isNoId()){
      y = addParameter(IntervalIntDomain("int"), "y");
    }
    if (autoClose)
      close();
    
    // Post parameter constraints
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(predicateVariable(IntervalIntDomain(1,1, "int")));
      token_constraint(eq, vars);
    }
  }
  
  
  // GamePlay.nddl:221 TestActions
  void TestActions::constructor() {
    AgentActions::constructor();
  }
  
  // GamePlay.nddl:221 TestActions
  class TestActionsFactory19: public ObjectFactory {
  public:
    TestActionsFactory19(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 0);
      TestActionsId instance = (new TestActions(planDb, objectType, objectName))->getId();
      instance->constructor();
      instance->handleDefaults();
      return instance;
    }
  };
  
  
  // GamePlay.nddl:238 ValueTimeline
  ValueTimeline::ValueTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "ValueTimeline", name) {
  }
  ValueTimeline::ValueTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  ValueTimeline::ValueTimeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "ValueTimeline", name) {}
  ValueTimeline::ValueTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void ValueTimeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:239 Holds
  ValueTimeline::Holds::Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  ValueTimeline::Holds::Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void ValueTimeline::Holds::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(value.isNoId()){
      value = addParameter(IntervalIntDomain("int"), "value");
    }
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:238 ValueTimeline
  void ValueTimeline::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode, "undefined");
  }
  
  // GamePlay.nddl:238 ValueTimeline
  class ValueTimelineFactory20: public ObjectFactory {
  public:
    ValueTimelineFactory20(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      ValueTimelineId instance = (new ValueTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:244 increment
  class TestActions$increment$19$0: public RuleInstance {
  public:
    TestActions$increment$19$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$TestActions$increment$19, TestActions$increment$19$0, TestActions.increment, "GamePlay.nddl,244");
  
  void TestActions$increment$19$0::handleExecute() {
    slave(Counter::Holds, Counter.Holds, v1, LabelStr("any"));
    slave(Counter::Holds, Counter.Holds, v2, LabelStr("ends"));
    ends(this, v2);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("v1")), std::string("start")));
      vars.push_back(var(getId(),std::string("start")));
      rule_constraint(precedes, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("start")));
      vars.push_back(varfromtok(tok(getId(), std::string("v1")), std::string("end")));
      rule_constraint(precedes, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("v1")), std::string("value")));
      vars.push_back(ruleVariable(IntervalIntDomain(1,1, "int")));
      vars.push_back(varfromtok(tok(getId(), std::string("v2")), std::string("value")));
      rule_constraint(addEq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(ruleVariable(IntervalIntDomain(1, 2, "int")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("v1")), std::string("end")));
      vars.push_back(varfromtok(tok(getId(), std::string("v2")), std::string("start")));
      rule_constraint(concurrent, vars);
    }
  }
  
  // GamePlay.nddl:258 multiply
  class TestActions$multiply$20$0: public RuleInstance {
  public:
    TestActions$multiply$20$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$TestActions$multiply$20, TestActions$multiply$20$0, TestActions.multiply, "GamePlay.nddl,258");
  
  void TestActions$multiply$20$0::handleExecute() {
    slave(ValueTimeline::Holds, ValueTimeline.Holds, v1, LabelStr("contained_by"));
    contained_by(this, v1);
    slave(ValueTimeline::Holds, ValueTimeline.Holds, v2, LabelStr("any"));
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(ruleVariable(IntervalIntDomain(1,1, "int")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("v1")), std::string("end")));
      vars.push_back(var(getId(),std::string("end")));
      rule_constraint(concurrent, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("v1")), std::string("end")));
      vars.push_back(varfromtok(tok(getId(), std::string("v2")), std::string("start")));
      rule_constraint(concurrent, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("v1")), std::string("value")));
      vars.push_back(var(getId(),std::string("multiplier")));
      vars.push_back(varfromtok(tok(getId(), std::string("v2")), std::string("value")));
      rule_constraint(mulEq, vars);
    }
  }
  
  // GamePlay.nddl:275 iterator
  class TestActions$iterator$21$0: public RuleInstance {
  public:
    TestActions$iterator$21$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  class TestActions$iterator$21$0$0: public RuleInstance {
  public:
    TestActions$iterator$21$0$0(const RuleInstanceId& parent, const std::vector<ConstrainedVariableId>& vars, const bool positive)
    : RuleInstance(parent, vars, positive){}
    void handleExecute();
  };
  
  class TestActions$iterator$21$0$0$0: public RuleInstance {
  public:
    TestActions$iterator$21$0$0$0(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$TestActions$iterator$21, TestActions$iterator$21$0, TestActions.iterator, "GamePlay.nddl,275");
  
  void TestActions$iterator$21$0::handleExecute() {
    slave(Counter::Holds, Counter.Holds, c, LabelStr("starts"));
    starts(this, c);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(ruleVariable(IntervalIntDomain(2,2, "int")));
      rule_constraint(leq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("c")), std::string("value")));
      vars.push_back(var(getId(),std::string("counter")));
      rule_constraint(eq, vars);
    }
    addChildRule(new TestActions$iterator$21$0$0(m_id, makeScope(var(getId(),std::string("end"))),true));
  }
  void TestActions$iterator$21$0$0::handleExecute() {
    localVar(BoolDomain(), done, true);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("done")));
      vars.push_back(var(getId(),std::string("counter")));
      vars.push_back(ruleVariable(IntervalIntDomain(0,0, "int")));
      rule_constraint(testLEQ, vars);
    }
    addChildRule(new TestActions$iterator$21$0$0$0(m_id, var(getId(),std::string("done")), BoolDomain(false, "bool"), true));
  }
  void TestActions$iterator$21$0$0$0::handleExecute() {
    localSlave(iterator, i, "any");
    sameObject(object, i);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("i")), std::string("counter")));
      vars.push_back(ruleVariable(IntervalIntDomain(1,1, "int")));
      vars.push_back(var(getId(),std::string("counter")));
      rule_constraint(addEq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("end")));
      vars.push_back(ruleVariable(IntervalIntDomain(1, 2, "int")));
      vars.push_back(varfromtok(tok(getId(), std::string("i")), std::string("start")));
      rule_constraint(temporalDistance, vars);
    }
  }
  
  // GamePlay.nddl:291 Holds
  class ValueTimeline$Holds$22$0: public RuleInstance {
  public:
    ValueTimeline$Holds$22$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  class ValueTimeline$Holds$22$0$0: public RuleInstance {
  public:
    ValueTimeline$Holds$22$0$0(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  class ValueTimeline$Holds$22$0$1: public RuleInstance {
  public:
    ValueTimeline$Holds$22$0$1(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$ValueTimeline$Holds$22, ValueTimeline$Holds$22$0, ValueTimeline.Holds, "GamePlay.nddl,291");
  
  void ValueTimeline$Holds$22$0::handleExecute() {
    addChildRule(new ValueTimeline$Holds$22$0$0(m_id, var(getId(),std::string("value")), IntervalIntDomain(100,100, "int"), true));
    addChildRule(new ValueTimeline$Holds$22$0$1(m_id, var(getId(),std::string("value")), IntervalIntDomain(1000,1000, "int"), true));
  }
  void ValueTimeline$Holds$22$0$0::handleExecute() {
    slave(TestActions::increment, TestActions.increment, action, LabelStr("starts"));
    starts(this, action);
  }
  void ValueTimeline$Holds$22$0$1::handleExecute() {
    slave(TestActions::multiply, TestActions.multiply, action, LabelStr("contains"));
    contains(this, action);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("action")), std::string("multiplier")));
      vars.push_back(ruleVariable(IntervalIntDomain(10,10, "int")));
      rule_constraint(eq, vars);
    }
  }
  
  
  // GamePlay.nddl:302 Valve
  Valve::Valve(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "Valve", name) {
  }
  Valve::Valve(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  Valve::Valve(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "Valve", name) {}
  Valve::Valve(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void Valve::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:303 Open
  Valve::Open::Open(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  Valve::Open::Open(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void Valve::Open::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:304 Closed
  Valve::Closed::Closed(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  Valve::Closed::Closed(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void Valve::Closed::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:302 Valve
  void Valve::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode, "Closed");
  }
  
  // GamePlay.nddl:302 Valve
  class ValveFactory21: public ObjectFactory {
  public:
    ValveFactory21(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      ValveId instance = (new Valve(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:308 Open
  class Valve$Open$23$0: public RuleInstance {
  public:
    Valve$Open$23$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$Valve$Open$23, Valve$Open$23$0, Valve.Open, "GamePlay.nddl,308");
  
  void Valve$Open$23$0::handleExecute() {
    localSlave(Closed, slave48, "meets");
    sameObject(object, slave48);
    meets(this, slave48);
    localSlave(Closed, slave49, "met_by");
    sameObject(object, slave49);
    met_by(this, slave49);
  }
  
  // GamePlay.nddl:313 Closed
  class Valve$Closed$24$0: public RuleInstance {
  public:
    Valve$Closed$24$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$Valve$Closed$24, Valve$Closed$24$0, Valve.Closed, "GamePlay.nddl,313");
  
  void Valve$Closed$24$0::handleExecute() {
    localSlave(Open, slave50, "meets");
    sameObject(object, slave50);
    meets(this, slave50);
    localSlave(Open, slave51, "met_by");
    sameObject(object, slave51);
    met_by(this, slave51);
  }
  
  
  // GamePlay.nddl:318 ValveController
  ValveController::ValveController(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "ValveController", name) {
  }
  ValveController::ValveController(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  ValveController::ValveController(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "ValveController", name) {}
  ValveController::ValveController(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void ValveController::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:319 Inactive
  ValveController::Inactive::Inactive(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  ValveController::Inactive::Inactive(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void ValveController::Inactive::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:320 InconsistentSlavesInExecution
  ValveController::InconsistentSlavesInExecution::InconsistentSlavesInExecution(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  ValveController::InconsistentSlavesInExecution::InconsistentSlavesInExecution(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void ValveController::InconsistentSlavesInExecution::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:321 ValveStaysClosed
  ValveController::ValveStaysClosed::ValveStaysClosed(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  ValveController::ValveStaysClosed::ValveStaysClosed(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void ValveController::ValveStaysClosed::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:318 ValveController
  void ValveController::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode, "Inactive");
  }
  
  // GamePlay.nddl:318 ValveController
  class ValveControllerFactory22: public ObjectFactory {
  public:
    ValveControllerFactory22(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      ValveControllerId instance = (new ValveController(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:325 InconsistentSlavesInExecution
  class ValveController$InconsistentSlavesInExecution$25$0: public RuleInstance {
  public:
    ValveController$InconsistentSlavesInExecution$25$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  class ValveController$InconsistentSlavesInExecution$25$0$0: public RuleInstance {
  public:
    ValveController$InconsistentSlavesInExecution$25$0$0(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$ValveController$InconsistentSlavesInExecution$25, ValveController$InconsistentSlavesInExecution$25$0, ValveController.InconsistentSlavesInExecution, "GamePlay.nddl,325");
  
  void ValveController$InconsistentSlavesInExecution$25$0::handleExecute() {
    localVar(BoolDomain(), readyToClose, true);
    slave(Valve::Open, Valve.Open, slave52, LabelStr("contained_by"));
    contained_by(this, slave52);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("start")));
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(var(getId(),std::string("end")));
      rule_constraint(addEq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("readyToClose")));
      vars.push_back(ruleVariable(IntervalIntDomain(5,5, "int")));
      vars.push_back(var(getId(),std::string("duration")));
      rule_constraint(testLEQ, vars);
    }
    addChildRule(new ValveController$InconsistentSlavesInExecution$25$0$0(m_id, var(getId(),std::string("readyToClose")), BoolDomain(true, "bool"), true));
  }
  void ValveController$InconsistentSlavesInExecution$25$0$0::handleExecute() {
    slave(Valve::Closed, Valve.Closed, slave53, LabelStr("contained_by"));
    contained_by(this, slave53);
  }
  
  // GamePlay.nddl:337 ValveStaysClosed
  class ValveController$ValveStaysClosed$26$0: public RuleInstance {
  public:
    ValveController$ValveStaysClosed$26$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$ValveController$ValveStaysClosed$26, ValveController$ValveStaysClosed$26$0, ValveController.ValveStaysClosed, "GamePlay.nddl,337");
  
  void ValveController$ValveStaysClosed$26$0::handleExecute() {
    slave(Valve::Closed, Valve.Closed, slave54, LabelStr("contained_by"));
    contained_by(this, slave54);
  }
  
  
  // GamePlay.nddl:345 MotionSimulator
  MotionSimulator::MotionSimulator(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "MotionSimulator", name) {
  }
  MotionSimulator::MotionSimulator(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  MotionSimulator::MotionSimulator(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "MotionSimulator", name) {}
  MotionSimulator::MotionSimulator(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void MotionSimulator::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:346 Holds
  MotionSimulator::Holds::Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  MotionSimulator::Holds::Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void MotionSimulator::Holds::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(value.isNoId()){
      value = addParameter(IntervalIntDomain("int"), "value");
    }
    if (autoClose)
      close();
    
    // Post parameter constraints
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(predicateVariable(IntervalIntDomain(1,1, "int")));
      token_constraint(defaultOnCommit, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("value")));
      vars.push_back(predicateVariable(IntervalIntDomain(0,0, "int")));
      token_constraint(defaultOnCommit, vars);
    }
  }
  
  
  // GamePlay.nddl:345 MotionSimulator
  void MotionSimulator::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode, "Holds");
  }
  
  // GamePlay.nddl:345 MotionSimulator
  class MotionSimulatorFactory23: public ObjectFactory {
  public:
    MotionSimulatorFactory23(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      MotionSimulatorId instance = (new MotionSimulator(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  
  // GamePlay.nddl:355 HorizontalControl
  HorizontalControl::HorizontalControl(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "HorizontalControl", name) {
  }
  HorizontalControl::HorizontalControl(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  HorizontalControl::HorizontalControl(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "HorizontalControl", name) {}
  HorizontalControl::HorizontalControl(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void HorizontalControl::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:356 Holds
  HorizontalControl::Holds::Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  HorizontalControl::Holds::Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void HorizontalControl::Holds::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(value.isNoId()){
      value = addParameter(IntervalIntDomain("int"), "value");
    }
    if (autoClose)
      close();
    
    // Post parameter constraints
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("value")));
      vars.push_back(predicateVariable(IntervalIntDomain(0,0, "int")));
      token_constraint(defaultOnCommit, vars);
    }
  }
  
  
  // GamePlay.nddl:355 HorizontalControl
  void HorizontalControl::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode, "Holds");
  }
  
  // GamePlay.nddl:355 HorizontalControl
  class HorizontalControlFactory24: public ObjectFactory {
  public:
    HorizontalControlFactory24(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      HorizontalControlId instance = (new HorizontalControl(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:376 Holds
  class MotionSimulator$Holds$27$0: public RuleInstance {
  public:
    MotionSimulator$Holds$27$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  class MotionSimulator$Holds$27$0$0: public RuleInstance {
  public:
    MotionSimulator$Holds$27$0$0(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  class MotionSimulator$Holds$27$0$1: public RuleInstance {
  public:
    MotionSimulator$Holds$27$0$1(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  class MotionSimulator$Holds$27$0$2: public RuleInstance {
  public:
    MotionSimulator$Holds$27$0$2(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  class MotionSimulator$Holds$27$0$3: public RuleInstance {
  public:
    MotionSimulator$Holds$27$0$3(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$MotionSimulator$Holds$27, MotionSimulator$Holds$27$0, MotionSimulator.Holds, "GamePlay.nddl,376");
  
  void MotionSimulator$Holds$27$0::handleExecute() {
    addChildRule(new MotionSimulator$Holds$27$0$0(m_id, var(getId(),std::string("start")), IntervalIntDomain(1,1, "int"), true));
    addChildRule(new MotionSimulator$Holds$27$0$1(m_id, var(getId(),std::string("value")), IntervalIntDomain(1,1, "int"), true));
    addChildRule(new MotionSimulator$Holds$27$0$2(m_id, var(getId(),std::string("value")), IntervalIntDomain(2,2, "int"), true));
    addChildRule(new MotionSimulator$Holds$27$0$3(m_id, var(getId(),std::string("value")), IntervalIntDomain(3,3, "int"), true));
  }
  void MotionSimulator$Holds$27$0$0::handleExecute() {
    slave(HorizontalControl::Holds, HorizontalControl.Holds, slave55, LabelStr("contained_by"));
    contained_by(this, slave55);
    slave(Valve::Open, Valve.Open, slave56, LabelStr("equals"));
    equals(this, slave56);
    slave(HorizontalControl::Holds, HorizontalControl.Holds, slave57, LabelStr("equals"));
    equals(this, slave57);
  }
  void MotionSimulator$Holds$27$0$1::handleExecute() {
    slave(HorizontalControl::Holds, HorizontalControl.Holds, hc, LabelStr("contained_by"));
    contained_by(this, hc);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("hc")), std::string("value")));
      vars.push_back(ruleVariable(IntervalIntDomain(1,1, "int")));
      rule_constraint(eq, vars);
    }
  }
  void MotionSimulator$Holds$27$0$2::handleExecute() {
    slave(HorizontalControl::Holds, HorizontalControl.Holds, hc, LabelStr("any"));
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("hc")), std::string("value")));
      vars.push_back(ruleVariable(IntervalIntDomain(2,2, "int")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("hc")), std::string("start")));
      vars.push_back(var(getId(),std::string("start")));
      rule_constraint(precedes, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("start")));
      vars.push_back(varfromtok(tok(getId(), std::string("hc")), std::string("end")));
      rule_constraint(precedes, vars);
    }
  }
  void MotionSimulator$Holds$27$0$3::handleExecute() {
    slave(HorizontalControl::Holds, HorizontalControl.Holds, hc, LabelStr("any"));
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("hc")), std::string("value")));
      vars.push_back(ruleVariable(IntervalIntDomain(3,3, "int")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("hc")), std::string("start")));
      vars.push_back(var(getId(),std::string("end")));
      rule_constraint(precedes, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("end")));
      vars.push_back(varfromtok(tok(getId(), std::string("hc")), std::string("end")));
      rule_constraint(precedes, vars);
    }
  }
  
  
  // GamePlay.nddl:414 ScalabilityTestTimeline
  ScalabilityTestTimeline::ScalabilityTestTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "ScalabilityTestTimeline", name) {
  }
  ScalabilityTestTimeline::ScalabilityTestTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  ScalabilityTestTimeline::ScalabilityTestTimeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "ScalabilityTestTimeline", name) {}
  ScalabilityTestTimeline::ScalabilityTestTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void ScalabilityTestTimeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if(n.isNoId()){
      n = addVariable(IntervalIntDomain("int"), "n");
    }
    if(d.isNoId()){
      d = addVariable(IntervalIntDomain("int"), "d");
    }
    if(maxD.isNoId()){
      maxD = addVariable(IntervalIntDomain("int"), "maxD");
    }
    if(c.isNoId()){
      c = addVariable(IntervalIntDomain("int"), "c");
    }
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:419 Holds
  ScalabilityTestTimeline::Holds::Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  ScalabilityTestTimeline::Holds::Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void ScalabilityTestTimeline::Holds::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:414 ScalabilityTestTimeline
  void ScalabilityTestTimeline::constructor(int _n, int _d, int _maxD, int _c, const LabelStr& _mode) {
    AgentTimeline::constructor(_mode, "Holds");
    n = addVariable(IntervalIntDomain(_n, _n, "int"), "n");
    d = addVariable(IntervalIntDomain(_d, _d, "int"), "d");
    maxD = addVariable(IntervalIntDomain(_maxD, _maxD, "int"), "maxD");
    c = addVariable(IntervalIntDomain(_c, _c, "int"), "c");
  }
  
  // GamePlay.nddl:414 ScalabilityTestTimeline
  class ScalabilityTestTimelineFactory25: public ObjectFactory {
  public:
    ScalabilityTestTimelineFactory25(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 5);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("int")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to int");
      check_error(arguments[0]->isSingleton());
      int _n((int)arguments[0]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[1], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("int")), 
                  "Cannot convert " + arguments[1]->getTypeName().toString() + " to int");
      check_error(arguments[1]->isSingleton());
      int _d((int)arguments[1]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[2], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("int")), 
                  "Cannot convert " + arguments[2]->getTypeName().toString() + " to int");
      check_error(arguments[2]->isSingleton());
      int _maxD((int)arguments[2]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[3], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("int")), 
                  "Cannot convert " + arguments[3]->getTypeName().toString() + " to int");
      check_error(arguments[3]->isSingleton());
      int _c((int)arguments[3]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[4], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[4]->getTypeName().toString() + " to Mode");
      check_error(arguments[4]->isSingleton());
      LabelStr _mode((LabelStr)arguments[4]->getSingletonValue());
      
      ScalabilityTestTimelineId instance = (new ScalabilityTestTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_n, _d, _maxD, _c, _mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:414 ScalabilityTestTimeline
  void ScalabilityTestTimeline::constructor(int _n, int _d, int _c, const LabelStr& _mode) {
    AgentTimeline::constructor(_mode, "Holds");
    n = addVariable(IntervalIntDomain(_n, _n, "int"), "n");
    d = addVariable(IntervalIntDomain(_d, _d, "int"), "d");
    maxD = addVariable(IntervalIntDomain(_d, _d, "int"), "maxD");
    c = addVariable(IntervalIntDomain(_c, _c, "int"), "c");
  }
  
  // GamePlay.nddl:414 ScalabilityTestTimeline
  class ScalabilityTestTimelineFactory26: public ObjectFactory {
  public:
    ScalabilityTestTimelineFactory26(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 4);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("int")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to int");
      check_error(arguments[0]->isSingleton());
      int _n((int)arguments[0]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[1], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("int")), 
                  "Cannot convert " + arguments[1]->getTypeName().toString() + " to int");
      check_error(arguments[1]->isSingleton());
      int _d((int)arguments[1]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[2], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("int")), 
                  "Cannot convert " + arguments[2]->getTypeName().toString() + " to int");
      check_error(arguments[2]->isSingleton());
      int _c((int)arguments[2]->getSingletonValue());
      
      check_error(AbstractDomain::canBeCompared(*arguments[3], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[3]->getTypeName().toString() + " to Mode");
      check_error(arguments[3]->isSingleton());
      LabelStr _mode((LabelStr)arguments[3]->getSingletonValue());
      
      ScalabilityTestTimelineId instance = (new ScalabilityTestTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_n, _d, _c, _mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:439 Holds
  class ScalabilityTestTimeline$Holds$28$0: public RuleInstance {
  public:
    ScalabilityTestTimeline$Holds$28$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  class ScalabilityTestTimeline$Holds$28$0$0: public RuleInstance {
  public:
    ScalabilityTestTimeline$Holds$28$0$0(const RuleInstanceId& parent, const std::vector<ConstrainedVariableId>& vars, const bool positive)
    : RuleInstance(parent, vars, positive){}
    void handleExecute();
  };
  
  class ScalabilityTestTimeline$Holds$28$0$1: public RuleInstance {
  public:
    ScalabilityTestTimeline$Holds$28$0$1(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  class ScalabilityTestTimeline$Holds$28$0$1$0: public RuleInstance {
  public:
    ScalabilityTestTimeline$Holds$28$0$1$0(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  class ScalabilityTestTimeline$Holds$28$0$1$0$0: public RuleInstance {
  public:
    ScalabilityTestTimeline$Holds$28$0$1$0$0(const RuleInstanceId& parent, const std::vector<ConstrainedVariableId>& vars, const bool positive)
    : RuleInstance(parent, vars, positive){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$ScalabilityTestTimeline$Holds$28, ScalabilityTestTimeline$Holds$28$0, ScalabilityTestTimeline.Holds, "GamePlay.nddl,439");
  
  void ScalabilityTestTimeline$Holds$28$0::handleExecute() {
    localSlave(Holds, p, "met_by");
    sameObject(object, p);
    met_by(this, p);
    localSlave(Holds, s, "meets");
    sameObject(object, s);
    meets(this, s);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(varFromObject(std::string("object"), std::string("d"), false));
      rule_constraint(defaultOnCommit, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(varFromObject(std::string("object"), std::string("maxD"), false));
      rule_constraint(leq, vars);
    }
    addChildRule(new ScalabilityTestTimeline$Holds$28$0$0(m_id, makeScope(var(getId(),std::string("duration"))),true));
    addChildRule(new ScalabilityTestTimeline$Holds$28$0$1(m_id, varFromObject(std::string("object"), std::string("mode"), true), SymbolDomain(LabelStr("Internal"), "Mode"), true));
  }
  void ScalabilityTestTimeline$Holds$28$0$0::handleExecute() {
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(varFromObject(std::string("object"), std::string("d"), false));
      rule_constraint(eq, vars);
    }
  }
  void ScalabilityTestTimeline$Holds$28$0$1::handleExecute() {
    localVar(BoolDomain(), hasNeighbors, true);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("hasNeighbors")));
      vars.push_back(ruleVariable(IntervalIntDomain(0,0, "int")));
      vars.push_back(varFromObject(std::string("object"), std::string("c"), false));
      rule_constraint(testLT, vars);
    }
    addChildRule(new ScalabilityTestTimeline$Holds$28$0$1$0(m_id, var(getId(),std::string("hasNeighbors")), BoolDomain(true, "bool"), true));
  }
  void ScalabilityTestTimeline$Holds$28$0$1$0::handleExecute() {
    objectVar(ScalabilityTestTimeline, allTimelines, false, false);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varFromObject(std::string("object"), std::string("n"), false));
      vars.push_back(varFromObject(std::string("object"), std::string("c"), false));
      vars.push_back(varFromObject(std::string("allTimelines"), std::string("n"), false));
      rule_constraint(neighborhood, vars);
    }
    addChildRule(new ScalabilityTestTimeline$Holds$28$0$1$0$0(m_id, makeScope(var(getId(),std::string("object"))),true));
  }
  void ScalabilityTestTimeline$Holds$28$0$1$0$0::handleExecute() {
    {
      // Create a local domain based on the allTimelines objects
      const ObjectDomain& foreach_allTimelines =
        static_cast<const ObjectDomain&>(var(getId(), std::string("allTimelines"))->derivedDomain()); 
        
      if(!foreach_allTimelines.isEmpty()){
        // Post a locking constraint on allTimelines
        {
          std::vector<ConstrainedVariableId> loop_vars;
          loop_vars.push_back(var(getId(), std::string("allTimelines")));
          loop_vars.push_back(ruleVariable(foreach_allTimelines));
          rule_constraint(Lock, loop_vars);
        }
        std::list<double> foreach_allTimelines_values;
        foreach_allTimelines.getValues(foreach_allTimelines_values);
        // Translate into a set ordered by key to ensure reliable ordering across runs
        ObjectSet foreach_allTimelines_valuesByKey;
        for(std::list<double>::iterator it=foreach_allTimelines_values.begin();
          it!=foreach_allTimelines_values.end(); ++it){
          ScalabilityTestTimelineId t = *it;
          foreach_allTimelines_valuesByKey.insert(t);
        }
        // Process slaves
        for(ObjectSet::const_iterator it=foreach_allTimelines_valuesByKey.begin();
          it!=foreach_allTimelines_valuesByKey.end(); ++it){
          ScalabilityTestTimelineId o = *it;
          check_error(o.isValid());
          // Allocate a local variable for this singleton object
          loopVar(ScalabilityTestTimeline, o);
          slave(ScalabilityTestTimeline::Holds, ScalabilityTestTimeline.Holds, slave63, LabelStr("starts"));
          sameObject(o, slave63);
          starts(this, slave63);
          clearLoopVar("o");
        }
      }
    }
  }
  
  PRIORITY PRIORITYBaseDomain(){
    static PRIORITY sl_interval(0, 5, "PRIORITY");
    return(sl_interval);
  }
  
  OrienteerID OrienteerIDBaseDomain(){
    static OrienteerID sl_interval(0, 500, "OrienteerID");
    return(sl_interval);
  }
  
  PFLOAT PFLOATBaseDomain(){
    static PFLOAT sl_interval(0.0, +inf, "PFLOAT");
    return(sl_interval);
  }
  
  
  // GamePlay.nddl:483 OrienteeringTimeline
  OrienteeringTimeline::OrienteeringTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "OrienteeringTimeline", name) {
  }
  OrienteeringTimeline::OrienteeringTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  OrienteeringTimeline::OrienteeringTimeline(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "OrienteeringTimeline", name) {}
  OrienteeringTimeline::OrienteeringTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void OrienteeringTimeline::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:484 Inactive
  OrienteeringTimeline::Inactive::Inactive(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  OrienteeringTimeline::Inactive::Inactive(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void OrienteeringTimeline::Inactive::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(x.isNoId()){
      x = addParameter(PFLOATBaseDomain(), "x");
    }
    if(y.isNoId()){
      y = addParameter(PFLOATBaseDomain(), "y");
    }
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:488 Active
  OrienteeringTimeline::Active::Active(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  OrienteeringTimeline::Active::Active(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void OrienteeringTimeline::Active::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(x.isNoId()){
      x = addParameter(PFLOATBaseDomain(), "x");
    }
    if(y.isNoId()){
      y = addParameter(PFLOATBaseDomain(), "y");
    }
    if(priority.isNoId()){
      priority = addParameter(PRIORITYBaseDomain(), "priority");
    }
    if(fakeDuration.isNoId()){
      fakeDuration = addParameter(IntervalIntDomain("int"), "fakeDuration");
    }
    if(value.isNoId()){
      value = addParameter(OrienteerIDBaseDomain(), "value");
    }
    if(countertimeline.isNoId()){
      countertimeline = addParameter(ObjectDomain("Counter"), "countertimeline");
      completeObjectParam(Counter, countertimeline);
    }
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:483 OrienteeringTimeline
  void OrienteeringTimeline::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode);
  }
  
  // GamePlay.nddl:483 OrienteeringTimeline
  class OrienteeringTimelineFactory27: public ObjectFactory {
  public:
    OrienteeringTimelineFactory27(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      OrienteeringTimelineId instance = (new OrienteeringTimeline(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:503 Inactive
  class OrienteeringTimeline$Inactive$29$0: public RuleInstance {
  public:
    OrienteeringTimeline$Inactive$29$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  class OrienteeringTimeline$Inactive$29$0$0: public RuleInstance {
  public:
    OrienteeringTimeline$Inactive$29$0$0(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  class OrienteeringTimeline$Inactive$29$0$0$0: public RuleInstance {
  public:
    OrienteeringTimeline$Inactive$29$0$0$0(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$OrienteeringTimeline$Inactive$29, OrienteeringTimeline$Inactive$29$0, OrienteeringTimeline.Inactive, "GamePlay.nddl,503");
  
  void OrienteeringTimeline$Inactive$29$0::handleExecute() {
    localSlave(Active, slave64, "met_by");
    sameObject(object, slave64);
    met_by(this, slave64);
    localSlave(Active, slave65, "meets");
    sameObject(object, slave65);
    meets(this, slave65);
    addChildRule(new OrienteeringTimeline$Inactive$29$0$0(m_id, varFromObject(std::string("object"), std::string("mode"), true), SymbolDomain(LabelStr("Internal"), "Mode"), true));
  }
  void OrienteeringTimeline$Inactive$29$0$0::handleExecute() {
    localVar(BoolDomain(), shouldEnd, true);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("shouldEnd")));
      vars.push_back(var(getId(),std::string("end")));
      vars.push_back(var(getId(),std::string("missionEnd")));
      rule_constraint(testLEQ, vars);
    }
    addChildRule(new OrienteeringTimeline$Inactive$29$0$0$0(m_id, var(getId(),std::string("shouldEnd")), BoolDomain(true, "bool"), true));
  }
  void OrienteeringTimeline$Inactive$29$0$0$0::handleExecute() {
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(ruleVariable(IntervalIntDomain(4,4, "int")));
      rule_constraint(eq, vars);
    }
  }
  
  // GamePlay.nddl:514 Active
  class OrienteeringTimeline$Active$30$0: public RuleInstance {
  public:
    OrienteeringTimeline$Active$30$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  class OrienteeringTimeline$Active$30$0$0: public RuleInstance {
  public:
    OrienteeringTimeline$Active$30$0$0(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$OrienteeringTimeline$Active$30, OrienteeringTimeline$Active$30$0, OrienteeringTimeline.Active, "GamePlay.nddl,514");
  
  void OrienteeringTimeline$Active$30$0::handleExecute() {
    localSlave(Inactive, slave66, "met_by");
    sameObject(object, slave66);
    met_by(this, slave66);
    localSlave(Inactive, next, "meets");
    sameObject(object, next);
    meets(this, next);
    slave(Counter::Holds, Counter.Holds, hp, LabelStr("contained_by"));
    sameObject(countertimeline, hp);
    contained_by(this, hp);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("hp")), std::string("value")));
      vars.push_back(var(getId(),std::string("value")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("value")));
      vars.push_back(ruleVariable(IntervalIntDomain(499,499, "int")));
      rule_constraint(neq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("next")), std::string("x")));
      vars.push_back(var(getId(),std::string("x")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("next")), std::string("y")));
      vars.push_back(var(getId(),std::string("y")));
      rule_constraint(eq, vars);
    }
    addChildRule(new OrienteeringTimeline$Active$30$0$0(m_id, varFromObject(std::string("object"), std::string("mode"), true), SymbolDomain(LabelStr("Internal"), "Mode"), true));
  }
  void OrienteeringTimeline$Active$30$0$0::handleExecute() {
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(var(getId(),std::string("fakeDuration")));
      rule_constraint(eq, vars);
    }
  }
  
  
  // GamePlay.nddl:531 GraspObject
  GraspObject::GraspObject(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "GraspObject", name) {
  }
  GraspObject::GraspObject(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  GraspObject::GraspObject(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "GraspObject", name) {}
  GraspObject::GraspObject(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void GraspObject::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if(param.isNoId()){
      param = addVariable(IntervalIntDomain("int"), "param");
    }
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:534 Inactive
  GraspObject::Inactive::Inactive(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  GraspObject::Inactive::Inactive(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void GraspObject::Inactive::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:536 Active
  GraspObject::Active::Active(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  GraspObject::Active::Active(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void GraspObject::Active::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(x.isNoId()){
      x = addParameter(IntervalIntDomain("int"), "x");
    }
    if(y.isNoId()){
      y = addParameter(IntervalIntDomain("int"), "y");
    }
    if (autoClose)
      close();
  }
  
  
  // GamePlay.nddl:531 GraspObject
  void GraspObject::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode, "Inactive");
    param = addVariable(IntervalIntDomain(0, 0, "int"), "param");
  }
  
  // GamePlay.nddl:531 GraspObject
  class GraspObjectFactory28: public ObjectFactory {
  public:
    GraspObjectFactory28(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      GraspObjectId instance = (new GraspObject(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:547 Inactive
  class GraspObject$Inactive$31$0: public RuleInstance {
  public:
    GraspObject$Inactive$31$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$GraspObject$Inactive$31, GraspObject$Inactive$31$0, GraspObject.Inactive, "GamePlay.nddl,547");
  
  void GraspObject$Inactive$31$0::handleExecute() {
    localSlave(Active, slave69, "met_by");
    sameObject(object, slave69);
    met_by(this, slave69);
    localSlave(Active, slave70, "meets");
    sameObject(object, slave70);
    meets(this, slave70);
  }
  
  // GamePlay.nddl:552 Active
  class GraspObject$Active$32$0: public RuleInstance {
  public:
    GraspObject$Active$32$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$GraspObject$Active$32, GraspObject$Active$32$0, GraspObject.Active, "GamePlay.nddl,552");
  
  void GraspObject$Active$32$0::handleExecute() {
    localSlave(Inactive, slave71, "met_by");
    sameObject(object, slave71);
    met_by(this, slave71);
    localSlave(Inactive, slave72, "meets");
    sameObject(object, slave72);
    meets(this, slave72);
    slave(TestActions::doGrasp, TestActions.doGrasp, a, LabelStr("contains"));
    contains(this, a);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("a")), std::string("x")));
      vars.push_back(var(getId(),std::string("x")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("a")), std::string("y")));
      vars.push_back(var(getId(),std::string("y")));
      rule_constraint(eq, vars);
    }
  }
  
  
  // GamePlay.nddl:561 Position
  Position::Position(const PlanDatabaseId& planDatabase, const LabelStr& name)
   : AgentTimeline(planDatabase, "Position", name) {
  }
  Position::Position(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(planDatabase, type, name) {
  }
  Position::Position(const ObjectId& parent, const LabelStr& name)
   : AgentTimeline(parent, "Position", name) {}
  Position::Position(const ObjectId& parent, const LabelStr& type, const LabelStr& name)
   : AgentTimeline(parent, type, name) {}
  // default initialization of member variables
  void Position::handleDefaults(bool autoClose) {
    AgentTimeline::handleDefaults(false);
    if (autoClose)
      close();
  }
  
  
  
  // GamePlay.nddl:562 Holds
  Position::Holds::Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable, bool isFact, bool close)
   : NddlToken(planDatabase, name, rejectable, isFact, false) {
    handleDefaults(close);
  }
  
  Position::Holds::Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close)
   : NddlToken(parent, name, relation, false) {
    handleDefaults(close);
  }
  
  // default initialization of member variables
  void Position::Holds::handleDefaults(bool autoClose) {
    NddlToken::handleDefaults(false);
    if(x.isNoId()){
      x = addParameter(IntervalIntDomain("int"), "x");
    }
    if(y.isNoId()){
      y = addParameter(IntervalIntDomain("int"), "y");
    }
    if (autoClose)
      close();
    
    // Post parameter constraints
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("duration")));
      vars.push_back(predicateVariable(IntervalIntDomain(1,1, "int")));
      token_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("x")));
      vars.push_back(predicateVariable(IntervalIntDomain(0,0, "int")));
      token_constraint(defaultOnCommit, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("y")));
      vars.push_back(predicateVariable(IntervalIntDomain(0,0, "int")));
      token_constraint(defaultOnCommit, vars);
    }
  }
  
  
  // GamePlay.nddl:561 Position
  void Position::constructor(const LabelStr& _mode) {
    AgentTimeline::constructor(_mode, "Holds");
  }
  
  // GamePlay.nddl:561 Position
  class PositionFactory29: public ObjectFactory {
  public:
    PositionFactory29(const LabelStr& name): ObjectFactory(name){}
  private:
    ObjectId createInstance(const PlanDatabaseId& planDb,
                            const LabelStr& objectType, 
                            const LabelStr& objectName,
                            const std::vector<const AbstractDomain*>& arguments) const {
      check_error(arguments.size() == 1);
      check_error(AbstractDomain::canBeCompared(*arguments[0], 
                                                planDb->getConstraintEngine()->getCESchema()->baseDomain("Mode")), 
                  "Cannot convert " + arguments[0]->getTypeName().toString() + " to Mode");
      check_error(arguments[0]->isSingleton());
      LabelStr _mode((LabelStr)arguments[0]->getSingletonValue());
      
      PositionId instance = (new Position(planDb, objectType, objectName))->getId();
      instance->constructor(_mode);
      instance->handleDefaults();
      return instance;
    }
  };
  
  // GamePlay.nddl:573 doGrasp
  class TestActions$doGrasp$33$0: public RuleInstance {
  public:
    TestActions$doGrasp$33$0(const RuleId& rule, const TokenId& token, const PlanDatabaseId& planDb)
    : RuleInstance(rule, token, planDb){}
    void handleExecute();
  };
  
  class TestActions$doGrasp$33$0$0: public RuleInstance {
  public:
    TestActions$doGrasp$33$0$0(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  class TestActions$doGrasp$33$0$1: public RuleInstance {
  public:
    TestActions$doGrasp$33$0$1(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  class TestActions$doGrasp$33$0$1$0: public RuleInstance {
  public:
    TestActions$doGrasp$33$0$1$0(const RuleInstanceId& parent, const ConstrainedVariableId& var, const AbstractDomain& domain, const bool positive)
    : RuleInstance(parent, var, domain, positive){}
    void handleExecute();
  };
  
  DECLARE_AND_DEFINE_RULE(Rule$TestActions$doGrasp$33, TestActions$doGrasp$33$0, TestActions.doGrasp, "GamePlay.nddl,573");
  
  void TestActions$doGrasp$33$0::handleExecute() {
    localVar(BoolDomain(), shouldEnd, true);
    slave(GraspObject::Active, GraspObject.Active, g, LabelStr("contained_by"));
    contained_by(this, g);
    slave(Position::Holds, Position.Holds, p, LabelStr("met_by"));
    met_by(this, p);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("x")));
      vars.push_back(varfromtok(tok(getId(), std::string("g")), std::string("x")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("y")));
      vars.push_back(varfromtok(tok(getId(), std::string("g")), std::string("y")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(var(getId(),std::string("shouldEnd")));
      vars.push_back(var(getId(),std::string("x")));
      vars.push_back(varfromtok(tok(getId(), std::string("p")), std::string("x")));
      rule_constraint(testEQ, vars);
    }
    addChildRule(new TestActions$doGrasp$33$0$0(m_id, var(getId(),std::string("shouldEnd")), BoolDomain(false, "bool"), true));
    addChildRule(new TestActions$doGrasp$33$0$1(m_id, var(getId(),std::string("shouldEnd")), BoolDomain(true, "bool"), true));
  }
  void TestActions$doGrasp$33$0$0::handleExecute() {
    slave(TestActions::doGrasp, TestActions.doGrasp, s, LabelStr("before"));
    before(this, s);
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("s")), std::string("x")));
      vars.push_back(var(getId(),std::string("x")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("s")), std::string("y")));
      vars.push_back(var(getId(),std::string("y")));
      rule_constraint(eq, vars);
    }
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("s")), std::string("end")));
      vars.push_back(varfromtok(tok(getId(), std::string("g")), std::string("end")));
      rule_constraint(precedes, vars);
    }
  }
  void TestActions$doGrasp$33$0$1::handleExecute() {
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("g")), std::string("object.param")));
      vars.push_back(ruleVariable(IntervalIntDomain(0,0, "int")));
      rule_constraint(eq, vars);
    }
    addChildRule(new TestActions$doGrasp$33$0$1$0(m_id, varfromtok(tok(getId(), std::string("g")), std::string("object.param")), IntervalIntDomain(0,0, "int"), true));
  }
  void TestActions$doGrasp$33$0$1$0::handleExecute() {
    {
      std::vector<ConstrainedVariableId> vars;
      vars.push_back(varfromtok(tok(getId(), std::string("g")), std::string("end")));
      vars.push_back(var(getId(),std::string("end")));
      rule_constraint(concurrent, vars);
    }
  }
  
} // namespace NDDL


namespace NDDL {
  // Boot-strap code to initialize schema
  extern "C" SchemaId loadSchema(const SchemaId& schema,const RuleSchemaId& ruleSchema)
  {
    SchemaId id = schema;
    // Register Constraints
    // Invoke commands to populate schema with type definitions
    id->addObjectType("StringData", "Object");
    id->addObjectType("AgentTimeline", "Timeline");
    id->addObjectType("AgentActions", "Object");
    id->addObjectType("StateVariable", "AgentTimeline");
    id->addObjectType("CreatorTimeline", "AgentTimeline");
    id->addObjectType("ReciverTimeline", "AgentTimeline");
    id->addObjectType("EqReciverTimeline", "AgentTimeline");
    id->addObjectType("PlayerState", "AgentTimeline");
    id->addObjectType("Player", "AgentTimeline");
    id->addObjectType("Umpire", "AgentTimeline");
    id->addObjectType("NumberTimeline", "AgentTimeline");
    id->addObjectType("A_Timeline", "AgentTimeline");
    id->addObjectType("B_Timeline", "AgentTimeline");
    id->addObjectType("CyclicTimeline", "AgentTimeline");
    id->addObjectType("SensorTimeline", "AgentTimeline");
    id->addObjectType("DerivedTimeline", "AgentTimeline");
    id->addObjectType("Counter", "AgentTimeline");
    id->addObjectType("TestActions", "AgentActions");
    id->addObjectType("ValueTimeline", "AgentTimeline");
    id->addObjectType("Valve", "AgentTimeline");
    id->addObjectType("ValveController", "AgentTimeline");
    id->addObjectType("MotionSimulator", "AgentTimeline");
    id->addObjectType("HorizontalControl", "AgentTimeline");
    id->addObjectType("ScalabilityTestTimeline", "AgentTimeline");
    id->addObjectType("OrienteeringTimeline", "AgentTimeline");
    id->addObjectType("GraspObject", "AgentTimeline");
    id->addObjectType("Position", "AgentTimeline");
    id->addEnum("TokenStates");
    id->addMember("StringData", "string", "data");
    id->addEnum("Mode");
    id->addMember("AgentTimeline", "Mode", "mode");
    id->addMember("AgentTimeline", "string", "defaultPredicate");
    id->addPredicate("AgentTimeline.undefined");
    id->addPredicate("AgentTimeline.Terminated");
    id->addPredicate("StateVariable.Holds");
    id->addPredicate("CreatorTimeline.Gamma");
    id->addPredicate("CreatorTimeline.Delta");
    id->addPredicate("ReciverTimeline.Alpha");
    id->addPredicate("ReciverTimeline.Beta");
    id->addPredicate("EqReciverTimeline.Alpha");
    id->addPredicate("EqReciverTimeline.Beta");
    id->addEnum("Values");
    id->addPredicate("PlayerState.Hides");
    id->addPredicate("PlayerState.Shows");
    id->addMember("PlayerState.Shows", "Values", "value");
    id->addMember("Player", "PlayerState", "hand");
    id->addPredicate("Player.Wait");
    id->addPredicate("Player.One");
    id->addPredicate("Player.Two");
    id->addPredicate("Player.Three");
    id->addPredicate("Player.Draw");
    id->addMember("Umpire", "Player", "player1");
    id->addMember("Umpire", "Player", "player2");
    id->addPredicate("Umpire.PlayGame");
    id->addPredicate("Umpire.Wait");
    id->addPredicate("NumberTimeline.holds");
    id->addMember("NumberTimeline.holds", "int", "value");
    id->addPredicate("A_Timeline.pred");
    id->addPredicate("B_Timeline.pred1");
    id->addPredicate("B_Timeline.pred2");
    id->addPredicate("CyclicTimeline.pred");
    id->addPredicate("SensorTimeline.holds");
    id->addMember("SensorTimeline.holds", "int", "value");
    id->addPredicate("DerivedTimeline.holds");
    id->addMember("DerivedTimeline.holds", "int", "value");
    id->addPredicate("Counter.Holds");
    id->addMember("Counter.Holds", "int", "value");
    id->addPredicate("TestActions.increment");
    id->addPredicate("TestActions.multiply");
    id->addMember("TestActions.multiply", "int", "multiplier");
    id->addPredicate("TestActions.iterator");
    id->addMember("TestActions.iterator", "int", "counter");
    id->addPredicate("TestActions.doGrasp");
    id->addMember("TestActions.doGrasp", "int", "x");
    id->addMember("TestActions.doGrasp", "int", "y");
    id->addPredicate("ValueTimeline.Holds");
    id->addMember("ValueTimeline.Holds", "int", "value");
    id->addPredicate("Valve.Open");
    id->addPredicate("Valve.Closed");
    id->addPredicate("ValveController.Inactive");
    id->addPredicate("ValveController.InconsistentSlavesInExecution");
    id->addPredicate("ValveController.ValveStaysClosed");
    id->addPredicate("MotionSimulator.Holds");
    id->addMember("MotionSimulator.Holds", "int", "value");
    id->addPredicate("HorizontalControl.Holds");
    id->addMember("HorizontalControl.Holds", "int", "value");
    id->addMember("ScalabilityTestTimeline", "int", "n");
    id->addMember("ScalabilityTestTimeline", "int", "d");
    id->addMember("ScalabilityTestTimeline", "int", "maxD");
    id->addMember("ScalabilityTestTimeline", "int", "c");
    id->addPredicate("ScalabilityTestTimeline.Holds");
    id->addEnum("PRIORITY");
    id->addEnum("OrienteerID");
    id->addEnum("PFLOAT");
    id->addPredicate("OrienteeringTimeline.Inactive");
    id->addMember("OrienteeringTimeline.Inactive", "PFLOAT", "x");
    id->addMember("OrienteeringTimeline.Inactive", "PFLOAT", "y");
    id->addPredicate("OrienteeringTimeline.Active");
    id->addMember("OrienteeringTimeline.Active", "PFLOAT", "x");
    id->addMember("OrienteeringTimeline.Active", "PFLOAT", "y");
    id->addMember("OrienteeringTimeline.Active", "PRIORITY", "priority");
    id->addMember("OrienteeringTimeline.Active", "int", "fakeDuration");
    id->addMember("OrienteeringTimeline.Active", "OrienteerID", "value");
    id->addMember("OrienteeringTimeline.Active", "Counter", "countertimeline");
    id->addMember("GraspObject", "int", "param");
    id->addPredicate("GraspObject.Inactive");
    id->addPredicate("GraspObject.Active");
    id->addMember("GraspObject.Active", "int", "x");
    id->addMember("GraspObject.Active", "int", "y");
    id->addPredicate("Position.Holds");
    id->addMember("Position.Holds", "int", "x");
    id->addMember("Position.Holds", "int", "y");
    id->addValue("TokenStates", LabelStr("INACTIVE"));
    id->addValue("TokenStates", LabelStr("ACTIVE"));
    id->addValue("TokenStates", LabelStr("MERGED"));
    id->addValue("TokenStates", LabelStr("REJECTED"));
    id->addValue("Mode", LabelStr("External"));
    id->addValue("Mode", LabelStr("Internal"));
    id->addValue("Mode", LabelStr("Ignore"));
    id->addValue("Values", LabelStr("Rock"));
    id->addValue("Values", LabelStr("Paper"));
    id->addValue("Values", LabelStr("Scissors"));
    // Force allocation of model specific type factories
    // Allocate factories
    REGISTER_TOKEN_FACTORY(id,AgentTimeline::undefined::Factory);
    REGISTER_TOKEN_FACTORY(id,AgentTimeline::Terminated::Factory);
    REGISTER_TOKEN_FACTORY(id,StateVariable::Holds::Factory);
    REGISTER_TOKEN_FACTORY(id,CreatorTimeline::Gamma::Factory);
    REGISTER_TOKEN_FACTORY(id,CreatorTimeline::Delta::Factory);
    REGISTER_TOKEN_FACTORY(id,ReciverTimeline::Alpha::Factory);
    REGISTER_TOKEN_FACTORY(id,ReciverTimeline::Beta::Factory);
    REGISTER_TOKEN_FACTORY(id,EqReciverTimeline::Alpha::Factory);
    REGISTER_TOKEN_FACTORY(id,EqReciverTimeline::Beta::Factory);
    REGISTER_TOKEN_FACTORY(id,PlayerState::Hides::Factory);
    REGISTER_TOKEN_FACTORY(id,PlayerState::Shows::Factory);
    REGISTER_TOKEN_FACTORY(id,Player::Wait::Factory);
    REGISTER_TOKEN_FACTORY(id,Player::One::Factory);
    REGISTER_TOKEN_FACTORY(id,Player::Two::Factory);
    REGISTER_TOKEN_FACTORY(id,Player::Three::Factory);
    REGISTER_TOKEN_FACTORY(id,Player::Draw::Factory);
    REGISTER_TOKEN_FACTORY(id,Umpire::PlayGame::Factory);
    REGISTER_TOKEN_FACTORY(id,Umpire::Wait::Factory);
    REGISTER_TOKEN_FACTORY(id,NumberTimeline::holds::Factory);
    REGISTER_TOKEN_FACTORY(id,A_Timeline::pred::Factory);
    REGISTER_TOKEN_FACTORY(id,B_Timeline::pred1::Factory);
    REGISTER_TOKEN_FACTORY(id,B_Timeline::pred2::Factory);
    REGISTER_TOKEN_FACTORY(id,CyclicTimeline::pred::Factory);
    REGISTER_TOKEN_FACTORY(id,SensorTimeline::holds::Factory);
    REGISTER_TOKEN_FACTORY(id,DerivedTimeline::holds::Factory);
    REGISTER_TOKEN_FACTORY(id,Counter::Holds::Factory);
    REGISTER_TOKEN_FACTORY(id,TestActions::increment::Factory);
    REGISTER_TOKEN_FACTORY(id,TestActions::multiply::Factory);
    REGISTER_TOKEN_FACTORY(id,TestActions::iterator::Factory);
    REGISTER_TOKEN_FACTORY(id,TestActions::doGrasp::Factory);
    REGISTER_TOKEN_FACTORY(id,ValueTimeline::Holds::Factory);
    REGISTER_TOKEN_FACTORY(id,Valve::Open::Factory);
    REGISTER_TOKEN_FACTORY(id,Valve::Closed::Factory);
    REGISTER_TOKEN_FACTORY(id,ValveController::Inactive::Factory);
    REGISTER_TOKEN_FACTORY(id,ValveController::InconsistentSlavesInExecution::Factory);
    REGISTER_TOKEN_FACTORY(id,ValveController::ValveStaysClosed::Factory);
    REGISTER_TOKEN_FACTORY(id,MotionSimulator::Holds::Factory);
    REGISTER_TOKEN_FACTORY(id,HorizontalControl::Holds::Factory);
    REGISTER_TOKEN_FACTORY(id,ScalabilityTestTimeline::Holds::Factory);
    REGISTER_TOKEN_FACTORY(id,OrienteeringTimeline::Inactive::Factory);
    REGISTER_TOKEN_FACTORY(id,OrienteeringTimeline::Active::Factory);
    REGISTER_TOKEN_FACTORY(id,GraspObject::Inactive::Factory);
    REGISTER_TOKEN_FACTORY(id,GraspObject::Active::Factory);
    REGISTER_TOKEN_FACTORY(id,Position::Holds::Factory);
    REGISTER_TYPE_FACTORY(id->getCESchema(),Object, ObjectDomain("Object"));
    REGISTER_TYPE_FACTORY(id->getCESchema(),TokenStates, TokenStatesBaseDomain());
    REGISTER_OBJECT_FACTORY(id,StringDataFactory0, StringData:string);
    REGISTER_TYPE_FACTORY(id->getCESchema(),Mode, ModeBaseDomain());
    REGISTER_OBJECT_FACTORY(id,AgentTimelineFactory1, AgentTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,AgentTimelineFactory2, AgentTimeline:Mode:string);
    REGISTER_OBJECT_FACTORY(id,AgentActionsFactory3, AgentActions);
    REGISTER_OBJECT_FACTORY(id,StateVariableFactory4, StateVariable:Mode);
    REGISTER_OBJECT_FACTORY(id,StateVariableFactory5, StateVariable:Mode:string);
    REGISTER_OBJECT_FACTORY(id,CreatorTimelineFactory6, CreatorTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,ReciverTimelineFactory7, ReciverTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,EqReciverTimelineFactory8, EqReciverTimeline:Mode);
    REGISTER_TYPE_FACTORY(id->getCESchema(),Values, ValuesBaseDomain());
    REGISTER_OBJECT_FACTORY(id,PlayerStateFactory9, PlayerState:Mode);
    REGISTER_OBJECT_FACTORY(id,PlayerFactory10, Player:Mode:Mode);
    REGISTER_OBJECT_FACTORY(id,UmpireFactory11, Umpire:Player:Player:Mode);
    REGISTER_OBJECT_FACTORY(id,UmpireFactory11, Umpire:Player:Timeline:Mode);
    REGISTER_OBJECT_FACTORY(id,UmpireFactory11, Umpire:Player:AgentTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,UmpireFactory11, Umpire:Timeline:Player:Mode);
    REGISTER_OBJECT_FACTORY(id,UmpireFactory11, Umpire:Timeline:Timeline:Mode);
    REGISTER_OBJECT_FACTORY(id,UmpireFactory11, Umpire:Timeline:AgentTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,UmpireFactory11, Umpire:AgentTimeline:Player:Mode);
    REGISTER_OBJECT_FACTORY(id,UmpireFactory11, Umpire:AgentTimeline:Timeline:Mode);
    REGISTER_OBJECT_FACTORY(id,UmpireFactory11, Umpire:AgentTimeline:AgentTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,NumberTimelineFactory12, NumberTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,A_TimelineFactory13, A_Timeline:Mode);
    REGISTER_OBJECT_FACTORY(id,B_TimelineFactory14, B_Timeline:Mode);
    REGISTER_OBJECT_FACTORY(id,CyclicTimelineFactory15, CyclicTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,SensorTimelineFactory16, SensorTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,DerivedTimelineFactory17, DerivedTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,CounterFactory18, Counter:Mode);
    REGISTER_OBJECT_FACTORY(id,TestActionsFactory19, TestActions);
    REGISTER_OBJECT_FACTORY(id,ValueTimelineFactory20, ValueTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,ValveFactory21, Valve:Mode);
    REGISTER_OBJECT_FACTORY(id,ValveControllerFactory22, ValveController:Mode);
    REGISTER_OBJECT_FACTORY(id,MotionSimulatorFactory23, MotionSimulator:Mode);
    REGISTER_OBJECT_FACTORY(id,HorizontalControlFactory24, HorizontalControl:Mode);
    REGISTER_OBJECT_FACTORY(id,ScalabilityTestTimelineFactory25, ScalabilityTestTimeline:int:int:int:int:Mode);
    REGISTER_OBJECT_FACTORY(id,ScalabilityTestTimelineFactory26, ScalabilityTestTimeline:int:int:int:Mode);
    REGISTER_ITYPE_FACTORY(id->getCESchema(),PRIORITY, PRIORITYBaseDomain());
    REGISTER_ITYPE_FACTORY(id->getCESchema(),OrienteerID, OrienteerIDBaseDomain());
    REGISTER_ITYPE_FACTORY(id->getCESchema(),PFLOAT, PFLOATBaseDomain());
    REGISTER_OBJECT_FACTORY(id,OrienteeringTimelineFactory27, OrienteeringTimeline:Mode);
    REGISTER_OBJECT_FACTORY(id,GraspObjectFactory28, GraspObject:Mode);
    REGISTER_OBJECT_FACTORY(id,PositionFactory29, Position:Mode);
    // Allocate rules
    ruleSchema->registerRule((new Rule$A_Timeline$pred$13())->getId());
    ruleSchema->registerRule((new Rule$B_Timeline$pred1$14())->getId());
    ruleSchema->registerRule((new Rule$B_Timeline$pred2$15())->getId());
    ruleSchema->registerRule((new Rule$Counter$Holds$18())->getId());
    ruleSchema->registerRule((new Rule$CreatorTimeline$Gamma$0())->getId());
    ruleSchema->registerRule((new Rule$CyclicTimeline$pred$16())->getId());
    ruleSchema->registerRule((new Rule$EqReciverTimeline$Beta$2())->getId());
    ruleSchema->registerRule((new Rule$GraspObject$Active$32())->getId());
    ruleSchema->registerRule((new Rule$GraspObject$Inactive$31())->getId());
    ruleSchema->registerRule((new Rule$MotionSimulator$Holds$27())->getId());
    ruleSchema->registerRule((new Rule$NumberTimeline$holds$12())->getId());
    ruleSchema->registerRule((new Rule$OrienteeringTimeline$Active$30())->getId());
    ruleSchema->registerRule((new Rule$OrienteeringTimeline$Inactive$29())->getId());
    ruleSchema->registerRule((new Rule$Player$Draw$9())->getId());
    ruleSchema->registerRule((new Rule$Player$One$6())->getId());
    ruleSchema->registerRule((new Rule$Player$Three$8())->getId());
    ruleSchema->registerRule((new Rule$Player$Two$7())->getId());
    ruleSchema->registerRule((new Rule$Player$Wait$5())->getId());
    ruleSchema->registerRule((new Rule$PlayerState$Hides$3())->getId());
    ruleSchema->registerRule((new Rule$PlayerState$Shows$4())->getId());
    ruleSchema->registerRule((new Rule$ReciverTimeline$Beta$1())->getId());
    ruleSchema->registerRule((new Rule$ScalabilityTestTimeline$Holds$28())->getId());
    ruleSchema->registerRule((new Rule$SensorTimeline$holds$17())->getId());
    ruleSchema->registerRule((new Rule$TestActions$doGrasp$33())->getId());
    ruleSchema->registerRule((new Rule$TestActions$increment$19())->getId());
    ruleSchema->registerRule((new Rule$TestActions$iterator$21())->getId());
    ruleSchema->registerRule((new Rule$TestActions$multiply$20())->getId());
    ruleSchema->registerRule((new Rule$Umpire$PlayGame$10())->getId());
    ruleSchema->registerRule((new Rule$Umpire$Wait$11())->getId());
    ruleSchema->registerRule((new Rule$ValueTimeline$Holds$22())->getId());
    ruleSchema->registerRule((new Rule$Valve$Closed$24())->getId());
    ruleSchema->registerRule((new Rule$Valve$Open$23())->getId());
    ruleSchema->registerRule((new Rule$ValveController$InconsistentSlavesInExecution$25())->getId());
    ruleSchema->registerRule((new Rule$ValveController$ValveStaysClosed$26())->getId());
    return id;
  }
  
}
