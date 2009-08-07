#include "Assembly.hh"
#include "Utilities.hh"
#include "Constraints.hh"
#include "GoalManager.hh"
#include "OrienteeringSolver.hh"
#include "GreedyOpenConditionManager.hh"
#include "DbCore.hh"
#include "DbSolver.hh"
#include "TestMonitor.hh"
#include "Functions.hh"

#include "ModuleConstraintEngine.hh"
#include "Propagators.hh"
#include "ModulePlanDatabase.hh"
#include "ModuleRulesEngine.hh"
#include "ModuleTemporalNetwork.hh"
#include "ModuleSolvers.hh"
#include "ModuleNddl.hh"
#include "Nddl.hh"

// Support for required major plan database components
#include "PlanDatabase.hh"
#include "ConstraintEngine.hh"
#include "RulesEngine.hh"
#include "Filters.hh"

// Transactions
#include "DbClientTransactionPlayer.hh"
#include "DbClientTransactionLog.hh"
#include "NddlDefs.hh"

// Misc
#include "Utils.hh"

#include <fstream>
#include <sstream>

//#define USE_CODE_GENERATOR

#ifdef USE_CODE_GENERATOR
#include "Rule.hh"
#endif

namespace TREX {

  Assembly::Schema* Assembly::Schema::s_instance = NULL;

  Assembly::Assembly()
  {
    assertTrue(ALWAYS_FAIL, "Should never get here.");
  }

  Assembly::Assembly(const LabelStr& agentName, const LabelStr& reactorName)
    : m_agentName(agentName), m_reactorName(reactorName)
  {
    addModule((new ModuleConstraintEngine())->getId()); 
    addModule((new ModuleConstraintLibrary())->getId());
    addModule((new ModulePlanDatabase())->getId());
    addModule((new ModuleRulesEngine())->getId());
    addModule((new ModuleTemporalNetwork())->getId());
    addModule((new ModuleSolvers())->getId());
    addModule((new ModuleNddl())->getId());

    // Call base class initialization
    doStart();

    // Initialize member variables
    m_schema = ((EUROPA::Schema*)getComponent("Schema"))->getId();
    m_constraintEngine = ((ConstraintEngine*)getComponent("ConstraintEngine"))->getId();
    m_planDatabase = ((PlanDatabase*) getComponent("PlanDatabase"))->getId();
    m_rulesEngine = ((RulesEngine*) getComponent("RulesEngine"))->getId();
    m_ppw = NULL;

    // Add another propagator to handle propagation of commitment constraints. Will be scheduled last to ensure that
    // the nextwork is fully propagated before we make any commitments.
    new DefaultPropagator(LabelStr("OnCommit"), m_constraintEngine->getId());

    // Disable auto propagation
    m_constraintEngine->setAutoPropagation(false);

#ifdef USE_CODE_GENERATOR
    EUROPA::NDDL::loadSchema(getSchema(),((RuleSchema*)getComponent("RuleSchema"))->getId()); 
#endif

    // Register components
    Assembly::Schema* s = Assembly::Schema::instance();
    s->registerComponents(*this);

    // Finally, set the domain comparator explicitly
    DomainComparator::setComparator((EUROPA::Schema*) m_schema);
  }

  Assembly::~Assembly() 
  {  
    if(m_ppw == NULL)
      delete m_ppw;

    doShutdown(); 
  }

  bool Assembly::playTransactions(const char* txSource)
  {
    check_error(txSource != NULL, "NULL transaction source provided.");
    static bool isFile(true);

#ifdef USE_CODE_GENERATOR
    // Open transaction source 
    std::ifstream in(txSource);

    assertTrue(in, "Invalid transaction source '" + std::string(txSource) + "'.");

    // Play transaction
    DbClientTransactionPlayer transactionPlayer(m_planDatabase->getClient());
    transactionPlayer.play(in);
#else
#ifdef USE_NDDL_PARSER
    std::ifstream f1(findFile("NDDL.cfg").c_str());
    std::ifstream f2(findFile("temp_nddl_gen.cfg").c_str());
    TiXmlElement* iroot = NULL;
    if (f1.good()) {
      iroot = EUROPA::initXml(findFile("NDDL.cfg").c_str());
    } else if (f2.good()) {
      iroot = EUROPA::initXml(findFile("temp_nddl_gen.cfg").c_str());
    } else {
      checkError(false, "Could not find 'NDDL.cfg' or 'temp_nddl_gen.cfg'");
    }
    if (iroot) {
      for (TiXmlElement * ichild = iroot->FirstChildElement();
	   ichild != NULL;
	   ichild = ichild->NextSiblingElement()) {
	if (std::string(ichild->Value()) == "include") {
	  std::string path = std::string(ichild->Attribute("path"));
	  for (unsigned int i = 0; i < path.size(); i++) {
	    if (path[i] == ';') {
	      path[i] = ':';
	    }
	  }
	  getLanguageInterpreter("nddl")->getEngine()->getConfig()->setProperty("nddl.includePath", path);
	}
      }
    }
    try {
      std::string ret = executeScript("nddl", txSource, isFile);
      assertTrue(ret == "", "Parser failed with return: " + ret);
    } catch(std::string ex) {
      assertTrue(false, "Parser failed: " + ex);
    } catch(...) {
      assertTrue(false, "Parser failed with unknown exception.");
    }
#else
    executeScript("nddl-xml", txSource, isFile);
#endif
#endif

    return m_constraintEngine->constraintConsistent();
  }

  const std::string& Assembly::exportToPlanWorks(){
    static const std::string sl_reply("DONE");

    checkError(m_constraintEngine->constraintConsistent(), "Should be propagated.");
    getPPW()->write(TREX::Agent::instance()->getCurrentTick());

    return sl_reply;
  }

  DbWriter* Assembly::getPPW(){
    if(m_ppw == NULL){
      m_ppw = new DbWriter(m_agentName.toString(), m_reactorName.toString(), m_planDatabase, m_constraintEngine, m_rulesEngine);
    }

    return m_ppw;
  }

  Assembly::Schema::Schema(){
    if(s_instance != NULL)
      delete s_instance;

    s_instance = this;
  }

  Assembly::Schema::~Schema(){
    s_instance = NULL;
  }

  Assembly::Schema* Assembly::Schema::instance(){
    if(s_instance == NULL)
      new Assembly::Schema();

    return s_instance;
  }

  void Assembly::Schema::registerComponents(const Assembly& assembly){
    ConstraintEngineId constraintEngine = ((ConstraintEngine*) assembly.getComponent("ConstraintEngine"))->getId();
    checkError(constraintEngine.isValid(), "No ConstraintEngine registered");

    // Register constraints
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), SetDefaultOnCommit, "defaultOnCommit", "OnCommit");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), AbsMaxOnCommit, "absMaxOnCommit", "OnCommit");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), SetDefault, "default", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), SetDefault, "bind", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), LessThanConstraint, "lt", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TestLessThan, "testLT", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), Neighborhood, "neighborhood", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::CompletionMonitorConstraint, "assertCompleted", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::RejectionMonitorConstraint, "assertRejected", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsStarted, "isStarted", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsEnded, "isEnded", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsTimedOut, "isTimedOut", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsSucceded, "isSucceded", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsAborted, "isAborted", "Default");
    REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsPreempted, "isPreempted", "Default");

    // Orienteering solver component registration

    EUROPA::SOLVERS::ComponentFactoryMgr* cfm = (EUROPA::SOLVERS::ComponentFactoryMgr*)assembly.getComponent("ComponentFactoryMgr");
    REGISTER_FLAW_FILTER(cfm, TREX::GoalsOnlyFilter, GoalsOnly);
    REGISTER_FLAW_FILTER(cfm, TREX::NoGoalsFilter, NoGoals);
    REGISTER_FLAW_FILTER(cfm, TREX::DynamicGoalFilter, DynamicGoalFilter);
    REGISTER_FLAW_MANAGER(cfm, TREX::GoalManager, GoalManager);
    REGISTER_FLAW_MANAGER(cfm, TREX::GreedyOpenConditionManager, GreedyOpenConditionManager);
    REGISTER_COMPONENT_FACTORY(cfm, TREX::EuclideanCostEstimator, EuclideanCostEstimator);
    REGISTER_COMPONENT_FACTORY(cfm, TREX::OrienteeringSolver, OrienteeringSolver);
    REGISTER_COMPONENT_FACTORY(cfm, TREX::EuropaSolverAdapter, EuropaSolverAdapter);

    // Standard flaw filters used in DbCore
    REGISTER_FLAW_FILTER(cfm, DeliberationFilter, DeliberationFilter);
    REGISTER_FLAW_FILTER(cfm, SOLVERS::SingletonFilter, NotSingletonGuard);

    // Custom flaw handlers
    REGISTER_COMPONENT_FACTORY(cfm, TREX::TestConditionHandler, TestConditionHandler);
  }
}
