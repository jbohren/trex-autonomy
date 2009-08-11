
#ifndef H_Functions
#define H_Functions

#include <string>
#include "Constraint.hh"
#include "PlanDatabaseDefs.hh"
#include "Domains.hh"

using namespace EUROPA;

namespace TREX {

  /**
   * @brief A base class for execution extension functions for querying token execution state. Assumes that
   * the first argument is a bool and the second argument is a variable of a token.
   */
  class ExecutionFunction : public Constraint {
  public:
    ExecutionFunction(const LabelStr& name,
		      const LabelStr& propagatorName,
		      const ConstraintEngineId& constraintEngine,
		      const std::vector<ConstrainedVariableId>& variables);

  private:

    /**
     * @brief Utility to construct the constraint scope correctly, including accessing the clock variable
     */
    static std::vector<ConstrainedVariableId> makeScope(const std::vector<ConstrainedVariableId>& variables);

  protected:
    bool isStarted() const;
    bool isEnded();
    bool isStatus(const LabelStr& status);
    bool hasStatus();

    BoolDomain& m_result; // The function result
    const IntervalIntDomain& m_clock; // The agent clock
    const IntervalIntDomain& m_start; // The token start time
    const IntervalIntDomain& m_end; // The token end time
    const IntervalIntDomain& m_max_duration; // The token end time
    TokenId m_token; // The parent token
    LabelStr m_status; // The status, once computed, will be cached here. This avoid issues of relaxations and garbage collection

    static const LabelStr SUCCESS;
    static const LabelStr PREEMPTED;
    static const LabelStr ABORTED;
  };


  /**
   * @brief A function to test if a Behavior::Active token has started
   */
  class IsStarted : public ExecutionFunction {
  public:
    IsStarted(const LabelStr& name,
	      const LabelStr& propagatorName,
	      const ConstraintEngineId& constraintEngine,
	      const std::vector<ConstrainedVariableId>& variables);


  protected:
    virtual void handleExecute();
  };

  /**
   * @brief A function to test if a Behavior::Active token has ended
   */
  class IsEnded : public ExecutionFunction {
  public:
    IsEnded(const LabelStr& name,
	    const LabelStr& propagatorName,
	    const ConstraintEngineId& constraintEngine,
	    const std::vector<ConstrainedVariableId>& variables);


  protected:
    virtual void handleExecute();
  };


  /**
   * @brief A function to test if a Behavior::Active token has been timedOut
   */
  class IsTimedOut : public ExecutionFunction {
  public:
    IsTimedOut(const LabelStr& name,
	       const LabelStr& propagatorName,
	       const ConstraintEngineId& constraintEngine,
	       const std::vector<ConstrainedVariableId>& variables);
  protected:
    virtual void handleExecute();
    virtual void setSource(const ConstraintId& source_constraint);
  private:
    bool m_fired;
  };

  /**
   * @brief A base class for checking successor status
   */
  class IsStatus : public ExecutionFunction {
  public:
    IsStatus(const LabelStr& name,
	     const LabelStr& propagatorName,
	     const ConstraintEngineId& constraintEngine,
	     const std::vector<ConstrainedVariableId>& variables);
  protected:
    virtual void handleExecute();
    virtual bool checkStatus() = 0;
  };

  /**
   * @brief A function to test if a Behavior::Active token has been succeded
   */
  class IsSucceded : public IsStatus {
  public:
    IsSucceded(const LabelStr& name,
	       const LabelStr& propagatorName,
	       const ConstraintEngineId& constraintEngine,
	       const std::vector<ConstrainedVariableId>& variables);
  protected:
    virtual bool checkStatus();
  };

  /**
   * @brief A function to test if a Behavior::Active token has been preempted
   */
  class IsPreempted : public IsStatus {
  public:
    IsPreempted(const LabelStr& name,
		const LabelStr& propagatorName,
		const ConstraintEngineId& constraintEngine,
		const std::vector<ConstrainedVariableId>& variables);
  protected:
    virtual bool checkStatus();
  };

  /**
   * @brief A function to test if a Behavior::Active token has been aborted
   */
  class IsAborted : public IsStatus {
  public:
    IsAborted(const LabelStr& name,
	      const LabelStr& propagatorName,
	      const ConstraintEngineId& constraintEngine,
	      const std::vector<ConstrainedVariableId>& variables);
  protected:
    virtual bool checkStatus();
  };
}
#endif
