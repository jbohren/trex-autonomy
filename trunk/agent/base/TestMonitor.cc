#include "TestMonitor.hh"

namespace TREX {

  TestConditionHandler::TestConditionHandler(const TiXmlElement& config)
    : FlawHandler(config){}

  DecisionPointId TestConditionHandler::create(const DbClientId& client, const EntityId& flaw, const LabelStr& explanation) const {
    DecisionPoint* dp = new EUROPA::SOLVERS::OpenConditionDecisionPoint(client, flaw, *FlawHandler::m_configData, explanation);
    dp->setContext(m_context);
    return dp->getId();
  }

  bool TestConditionHandler::customStaticMatch(const EntityId& entity) const {
    return TestMonitor::isCondition(entity->getKey()); 
  }

  unsigned int TestConditionHandler::staticFilterCount() const {
    return 1 + FlawHandler::staticFilterCount();
  }

  TestMonitorConstraintBase::TestMonitorConstraintBase(const LabelStr& name,
						       const LabelStr& propagatorName,
						       const ConstraintEngineId& constraintEngine,
						       const std::vector<ConstrainedVariableId>& variables,
						       bool shouldBeCompleted)
    : Constraint(name, propagatorName, constraintEngine, variables){
    checkError(variables.size() == 1, variables.size());
    checkError(TokenId::convertable(variables[0]->parent()), variables[0]->toString());
    TokenId token = (TokenId) variables[0]->parent();
    TestMonitor::registerCondition(token->getKey(), token->toString(), shouldBeCompleted);
  }

  TestMonitorConstraintBase::~TestMonitorConstraintBase(){}

  CompletionMonitorConstraint::CompletionMonitorConstraint(const LabelStr& name,
							   const LabelStr& propagatorName,
							   const ConstraintEngineId& constraintEngine,
							   const std::vector<ConstrainedVariableId>& variables)
    : TestMonitorConstraintBase(name, propagatorName, constraintEngine, variables, true){}

  RejectionMonitorConstraint::RejectionMonitorConstraint(const LabelStr& name,
							 const LabelStr& propagatorName,
							 const ConstraintEngineId& constraintEngine,
							 const std::vector<ConstrainedVariableId>& variables)
    : TestMonitorConstraintBase(name, propagatorName, constraintEngine, variables, false){}

  TestMonitor::Entry::Entry(int key_, const std::string& label_, bool expectedValue_)
    :key(key_), label(label_), expectedValue(expectedValue_), actualValue(!expectedValue_), resolved(false){}

  bool TestMonitor::success(){
    // Iterate over all entries. Each one must be resolved and the expected and actual values must match
    for(std::list<TestMonitor::Entry>::const_iterator it = entries().begin(); it != entries().end(); ++it){
      const Entry& entry = *it;
      if(!entry.resolved || entry.expectedValue != entry.actualValue)
	return false;
    }

    return true;
  }

  void TestMonitor::reset(){
    entries().clear();
  }

  std::string TestMonitor::toString(){
    std::stringstream ss;
    for(std::list<TestMonitor::Entry>::const_iterator it = entries().begin(); it != entries().end(); ++it){
      const Entry& entry = *it;
      ss << (entry.resolved && entry.expectedValue == entry.actualValue ? "SUCCESS" : "FAILURE") << " for ";
      ss << entry.label << "(";
      ss << (entry.resolved ? "resolved" : "unresolved") << ", ";
      ss << (entry.expectedValue ? "expected to succeed" : " expected to fail") << ", ";
      ss << (entry.actualValue ? " suceeded" : "failed") << ")\n";
    }

    return ss.str();
  }

  void TestMonitor::registerCondition(int key, const std::string& label, bool expectedValue){
    // If there are no entries, then register a listener. The listener will be cleared up when the agent is deleted
    if(entries().empty())
      new TestMonitor::AgentListener();

    // Now add the entry
    entries().push_back(TestMonitor::Entry(key, label, expectedValue));
  }

  void TestMonitor::updateValue(int key, bool completed){
    for(std::list< Entry >::iterator it = entries().begin(); it != entries().end(); ++it){
      Entry& entry = *it;
      if(entry.key == key){
	entry.actualValue = completed;
	entry.resolved = true;
      }
    }
  }

  bool TestMonitor::isCondition(int key){
    for(std::list< Entry >::iterator it = entries().begin(); it != entries().end(); ++it){
      Entry& entry = *it;
      if(entry.key == key)
	return true;
    }

    return false;
  }

  void TestMonitor::AgentListener::notifyRejected(const TokenId& token){
    TestMonitor::updateValue(token->getKey(), false);
  }

  void TestMonitor::AgentListener::notifyCompleted(const TokenId& token){
    TestMonitor::updateValue(token->getKey(), true);
  }
}
