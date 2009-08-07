#include "Functions.hh"
#include "Utilities.hh"
#include "Token.hh"
#include "Schema.hh"
#include "PlanDatabase.hh"
#include "TokenVariable.hh"
#include "Object.hh"
#include "LabelStr.hh"
#include "DbCore.hh"
#include "Timeline.hh"

using namespace EUROPA;

namespace TREX {
  /*!< RESULT status codes */
  const LabelStr ExecutionFunction::SUCCESS("SUCCESS");
  const LabelStr ExecutionFunction::PREEMPTED("PREEMPTED");
  const LabelStr ExecutionFunction::ABORTED("ABORTED");

  ExecutionFunction::ExecutionFunction(const LabelStr& name,
				       const LabelStr& propagatorName,
				       const ConstraintEngineId& constraintEngine,
				       const std::vector<ConstrainedVariableId>& variables)
    : Constraint(name, propagatorName, constraintEngine, makeScope(variables)),
      m_result(static_cast<BoolDomain&>(getCurrentDomain(getScope()[0]))),
      m_clock(static_cast<IntervalIntDomain&>(getCurrentDomain(getScope()[1]))),
      m_start(static_cast<IntervalIntDomain&>(getCurrentDomain(getScope()[2]))),
      m_end(static_cast<IntervalIntDomain&>(getCurrentDomain(getScope()[3]))),
      m_max_duration(static_cast<IntervalIntDomain&>(getCurrentDomain(getScope()[4]))),
      m_token(getParentToken(getScope()[2])){
  }

  std::vector<ConstrainedVariableId> ExecutionFunction::makeScope(const std::vector<ConstrainedVariableId>& variables){
    checkError(variables.size() >= 2, "There must be at least 2 variables in the scope of this constraint. Instead there are only " << variables.size());

    // If the variable set is greater than 2 then we have already constructed the scope. This can be the case when constraints
    // are being migrated for example
    if(variables.size() > 2)
      return variables;

    static const LabelStr BEHAVIOR_ACTIVE("Behavior.Active");
    static const LabelStr PARAM_MAX_DURATION("max_duration");

    ConstrainedVariableId result_var = variables[0];
    ConstrainedVariableId token_var = variables[1];

    TokenId parent_token = getParentToken(token_var);

    checkError(parent_token.isId(),
	       "We assume the second variable is a variable of a Behavior::Active token, but it is not. " << token_var->toLongString());

    checkError(parent_token->getPlanDatabase()->getSchema()->isA(parent_token->getPredicateName(), BEHAVIOR_ACTIVE),
	       "We assume the second variable is a variable of a Behavior::Active token, but it is not. " << parent_token->toLongString());

    checkError(parent_token->getVariable(PARAM_MAX_DURATION, false).isId(), "Must have a max_duration parameter." << parent_token->toLongString());

    // Now build the new scope.
    std::vector<ConstrainedVariableId> new_scope;
    new_scope.push_back(result_var);
    new_scope.push_back(DbCore::getAgentClockVariable(parent_token->getPlanDatabase()));
    new_scope.push_back(parent_token->start());
    new_scope.push_back(parent_token->end());
    new_scope.push_back(parent_token->getVariable(PARAM_MAX_DURATION, false));
    new_scope.push_back(parent_token->getState());
    return new_scope;
  }

  bool ExecutionFunction::isStarted() const{
    return m_start.getUpperBound() <= m_clock.getLowerBound();
  }

  bool ExecutionFunction::isEnded(){
    return m_token->isCommitted() && m_end.getUpperBound() <= m_clock.getLowerBound() && hasStatus();
  }

  bool ExecutionFunction::isStatus(const LabelStr& status) {
    return status == m_status;
  }

  bool ExecutionFunction::hasStatus(){
    if(m_status == EMPTY_LABEL()){

      static const LabelStr PARAM_STATUS("status");
      static const LabelStr RELATION_MEETS("meets");
      static const LabelStr BEHAVIOR_INACTIVE("Inactive");

      // Obtain the successor token.
      TimelineId timeline = m_token->getObject()->lastDomain().getSingletonValue();
      DbCoreId db_core = DbCore::getInstance(m_token);
      TokenId successor_token = db_core->getValue(timeline, m_end.getUpperBound());

      if(successor_token.isId()){
	checkError(successor_token->getUnqualifiedPredicateName() == BEHAVIOR_INACTIVE, "Token is out of place. " << successor_token->toLongString());
	const AbstractDomain& successor_status = getCurrentDomain(successor_token->getVariable(PARAM_STATUS, false));
	if(successor_status.isSingleton())
	  m_status = successor_status.getSingletonValue();
      }
    }

    return m_status != EMPTY_LABEL();
  }


  IsStarted::IsStarted(const LabelStr& name,
			 const LabelStr& propagatorName,
			 const ConstraintEngineId& constraintEngine,
			 const std::vector<ConstrainedVariableId>& variables)
    : ExecutionFunction(name, propagatorName, constraintEngine, variables){} 

  void IsStarted::handleExecute(){
    if(isStarted())
      m_result.set(1);
  }

  IsEnded::IsEnded(const LabelStr& name,
			 const LabelStr& propagatorName,
			 const ConstraintEngineId& constraintEngine,
			 const std::vector<ConstrainedVariableId>& variables)
    : ExecutionFunction(name, propagatorName, constraintEngine, variables){} 

  void IsEnded::handleExecute(){
    if(isEnded())
      m_result.set(1);
  }

  IsTimedOut::IsTimedOut(const LabelStr& name,
			 const LabelStr& propagatorName,
			 const ConstraintEngineId& constraintEngine,
			 const std::vector<ConstrainedVariableId>& variables)
    : ExecutionFunction(name, propagatorName, constraintEngine, variables){} 

  void IsTimedOut::handleExecute(){
    // if it has started then we can test the bounds to see if it is necessarily restricted to be timed out
    if(isStarted()){
      if( (m_end.getLowerBound() - m_start.getUpperBound()) > m_max_duration.getUpperBound())
	 m_result.set(1);

      if(m_result.isEmpty())
	return;
    }

    // If ended we can evaluate the possibility that it certainly has not timed out
    if(isEnded()){
      if(m_end.getUpperBound() - m_start.getLowerBound() < m_max_duration.getUpperBound())
	m_result.set(0);
    }
  }


  IsStatus::IsStatus(const LabelStr& name,
			 const LabelStr& propagatorName,
			 const ConstraintEngineId& constraintEngine,
			 const std::vector<ConstrainedVariableId>& variables)
    : ExecutionFunction(name, propagatorName, constraintEngine, variables){} 

  void IsStatus::handleExecute(){
    if(isEnded()){
      if(checkStatus()){
	m_result.set(1);
      }
      else{
	m_result.set(0);
      }

      if(m_result.isEmpty())
	return;
    }
  }

  IsSucceded::IsSucceded(const LabelStr& name,
			 const LabelStr& propagatorName,
			 const ConstraintEngineId& constraintEngine,
			 const std::vector<ConstrainedVariableId>& variables)
    : IsStatus(name, propagatorName, constraintEngine, variables){} 

  bool IsSucceded::checkStatus() {return isStatus(SUCCESS); }

  IsAborted::IsAborted(const LabelStr& name,
			 const LabelStr& propagatorName,
			 const ConstraintEngineId& constraintEngine,
			 const std::vector<ConstrainedVariableId>& variables)
    : IsStatus(name, propagatorName, constraintEngine, variables){} 

  bool IsAborted::checkStatus() {return isStatus(ABORTED); }

  IsPreempted::IsPreempted(const LabelStr& name,
			 const LabelStr& propagatorName,
			 const ConstraintEngineId& constraintEngine,
			 const std::vector<ConstrainedVariableId>& variables)
    : IsStatus(name, propagatorName, constraintEngine, variables){} 

  bool IsPreempted::checkStatus() {return isStatus(PREEMPTED); }
}
