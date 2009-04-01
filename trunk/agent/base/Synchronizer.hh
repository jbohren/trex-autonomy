#ifndef H_Synchronizer
#define H_Synchronizer

#include "TREXDefs.hh"
#include "PlanDatabaseDefs.hh"
#include "RuleInstance.hh"

namespace TREX {

  /**
   * @brief Implements the synhronization algorithm and gathers all the components required for it
   */
  class Synchronizer {
  public:
    Synchronizer(const DbCoreId& _core);

    /**
     * @brief Resolve observations, goals and plan at the execution frontier.
     */
    bool resolve();

    /**
     * @brief Relax the database. Must retain commited values at the execution frontier, and keep current observations,
     * but want to relax goals and their implications.
     * @param discardCurrentValues If true, then current tokens, that belong to internal, non-persistent timelines, will be discarded.
     * @see resetRemainingTokens
     * @return true if successful, otherwise false
     */
    bool relax(bool discardCurrentValues);


    /** UTILITIES FOR ANALYSIS OF FAILURES **/
    std::string tokenResolutionFailure(const TokenId& tokenToResolve, const TokenId& merge_candidate) const;
    std::string propagationFailure() const;
    std::string localContextForConstrainedVariable(const ConstrainedVariableId& var) const;
    std::string tokenExtensionFailure(const TokenId& expectedToken) const;
    std::string analysisOfBlockingToken(const TokenId& tokenToResolve) const;
  private:

    /**
     * @brief Resolve all tokens at the execution frontier.
     */
    bool resolveTokens(unsigned int& stepCount);

    /**
     * @brief Resolve a specific token at the execution frontier
     */
    bool resolveToken(const TokenId& token, unsigned int& stepCount, const TokenId& mergeCandidate);

    /**
     * @brief Fire all rules in the execution frontier
     */
    bool fireRules(unsigned int& stepCount);

    /**
     * @brief Fire rules for a specific token
     */
    bool fireRules(const TokenId& token, unsigned int& stepCount);

    /**
     * @brief Fire a rule and its children as appropriiate
     */
    bool fire(const RuleInstanceId& r, unsigned int& stepCount);

    /**
     * @brief Merge a token if the option is there
     * @return true if it tried a merge, false if it did not
     */
    bool mergeToken(const TokenId& token, const TokenId& mergeCandidate);

    /**
     * @brief Insert a token and resolve its slaves
     */
    bool insertToken(const TokenId& token, unsigned int& stepCount);

    /**
     * @brief Batch insert of copied values for repair algorithm
     */
    bool insertCopiedValues();

    /**
     * @brief Utiltiy to place default tokens in empty slots.
     */
    bool completeInternalTimelines(unsigned int& stepCount);

    /**
     * @brief Utility to create and insert and undefined token in the given timeline and location
     */
    bool insertDefaultValue(const TimelineId& timeline, unsigned int& stepCount);

    /**
     * @brief Test for horizon part of synchronization scope.
     */  
    bool inTickHorizon(const TokenId& token, int currentTick);

    /**
     * @brief Test for a unit decision in the synchronization scope
     */
    bool isUnit(const TokenId& token, TokenId& mergeCandidate);

    /**
     * @brief Test for synchronization scope
     */
    bool inSynchScope(const TokenId& token, TokenId& mergeCandidate);

    /**
     * @brief Reset goals as part of repair
     */
    void resetGoals();

    /**
     * @brief Reset the buffered observations as part of repair
     */
    void resetObservations();

    /**
     * @brief Reset remaining tokens
     * @param discardCurrentValues If true, then current tokens, that belong to internal, non-persistent timelines, will be discarded.
     */
    void resetRemainingTokens(bool discardCurrentValues);

    /**
     * @brief test if the current value
     */
    bool isCurrent(const TokenId& token);

    /**
     * @brief Helper method to test if the token is on a persistent object
     */
    static bool isPersistent(const TokenId& token);

    /**
     * @brief Copy a value at the execution frontier. Used to re-apply the model.
     */
    void copyValue(const TokenId& source);

    /**
     * @brief Accessor to debug stream for debug output
     */
    std::ostream& getStream();

    /**
     * Utility to extract the underlying variable if this one is part of a merged token
     */
    static ConstrainedVariableId getActiveGuard(const ConstrainedVariableId& var);

    unsigned int m_stepCount;

    DbCoreId m_core;
    PlanDatabaseId m_db;
    const std::vector<TimelineId>& m_timelines;
    TokenSet& m_goals; /*!< Store all goals */
    TokenSet& m_actions; /*!< Store all actions */
    TokenSet& m_observations; /*!< Store received observations received */
    TokenSet& m_tokenAgenda; /*!< Buffer of tokens available for synchronization */
    TokenSet& m_committedTokens; /*!< Buffer of committed tokens */
  };

}
#endif
