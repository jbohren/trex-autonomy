#ifndef H_TestMonitor
#define H_TestMonitor

#include "Constraint.hh"
#include "Token.hh"

using namespace EUROPA;

/**
 * @file Contains the class definitions for supporting TREX test specifications in NDDL.
 * Usage in NDDL:
 *  assertCompleted(tokenLabel.state);
 *  assertRejected(tokenLabel.state);
 * 
 * Usage from test client:
 *  ASSERT_EQ(TestMonitor::success())
 *  std::cout << testMonitor::toString()
 *  TestMonitor::reset();
 */
namespace TREX {


  class TestMonitorConstraintBase : public Constraint {
  public:
    virtual ~TestMonitorConstraintBase();


    /**
     * @brief True if the base domain of the underlying token state variable is a singleton.
     */
    bool isResolved() const;

    /**
     * @brief True if the state variable of the token is a singleton and is not rejected.
     */
    bool isCompleted() const;

  protected:
    TestMonitorConstraintBase(const LabelStr& name,
			      const LabelStr& propagatorName,
			      const ConstraintEngineId& constraintEngine,
			      const std::vector<ConstrainedVariableId>& variables,
			      bool shouldBeCompleted_);

    void handleExecute();

    TokenId m_token;
    AbstractDomain& m_state;
    const bool shouldBeCompleted;
  };

  class CompletionMonitorConstraint: public TestMonitorConstraintBase {
  public:
    CompletionMonitorConstraint(const LabelStr& name,
				const LabelStr& propagatorName,
				const ConstraintEngineId& constraintEngine,
				const std::vector<ConstrainedVariableId>& variables);
  };

  class RejectionMonitorConstraint: public TestMonitorConstraintBase {
  public:
    RejectionMonitorConstraint(const LabelStr& name,
			       const LabelStr& propagatorName,
			       const ConstraintEngineId& constraintEngine,
			       const std::vector<ConstrainedVariableId>& variables);
  };

  class TestMonitor {
  public:
    /**
     * @brief Return true if all test criteria have been resolved and have where the actual value
     * is the expected value.
     */
    static bool success();

    /**
     * @brief Allows the set of test monitors to be cleared.
     */
    static void reset();

    /**
     * @brief Outputs the expected result
     */
    static std::string toString();

  private:
    friend class testMonitorConstraintBase;

    /**
     * @brief Call on creation of a constraint to register it for evaluation
     */
    static void registerConstraint(const TestMonitorConstraintBase& c);

    /**
     * @brief When a constraint is deleted, clean it up
     */
    static void unregisterConstraint(const TestMonitorConstraintBase& c);


    /**
     * @brief Defines the row structure for test monitor entries. This class is used by the TestMonitor.
     */
    class Entry {
    public:
      Entry(int key_, const std::string& label, bool expectedValue_);
      const int key; // Key of the constraint. Will be unique through all execution
      const std::string label; // The label used to describe the constraint. Derived from the underlying token predicate
      const bool expectedValue; // True if expected to be completed, false if expected to be rejected
      bool actualValue; // Obvious
      bool resolved;// True if the result is specified, otherwise false.
    };

    static std::list< Entry >& entries(){
      static std::list< Entry > sl_entries;
      return sl_entries;
    }

  };
}

#endif
