#ifndef H_Agent
#define H_Agent

/**
 * @author Conor McGann
 * @file Provides declaration for the Agent class. This is basically a composition of TeleoReactors. The Agent will
 * allocate reactors and connect them together according to their configuratiun requirements. It will also play the role
 * of middle-man to route observations from sender to receiver.
 * @note The Agent class is not thread safe. If run on a separate thread, it cannot be accessed further.
 */

#include "TREXDefs.hh"
#include "XMLUtils.hh"
#include "TeleoReactor.hh"
#include "Clock.hh"
#include "ObservationLogger.hh"
#include "PerformanceMonitor.hh"
#include "RStat.hh"
#include <vector>
#include <map>

namespace TREX {

  class SynchronizationFailure {
  public:
    SynchronizationFailure(){}
  };

  /**
   * @brief The Agent is an observer of messages from TeleoReactors. It is the message bus for distribution of observations
   * The agent employs 3 threads:
   * 1. The Clock Monitor Thread - used to read time and map to ticks. Drives clock event handling code throughout.
   * 2. The Reactor Execution Thread - used to power the main business of working flaws in the internal state of a reactor
   * 3. The Garbage Collector - used to archive crud accumulated through execution as it is no longer necessary.
   */
  class Agent {
  public:

    enum EventType {
      Notify = 0, /*!< For observations posted from a reactor */
      Request, /*!< For requests made on a reactor */
      Recall /*!< For recalls made on a reactor */
    };

    class Event {
    public:
      Event(TICK tick, EventType evType, const LabelStr& objectName, const LabelStr& predicateName);
      Event(const Event& org);
      TICK m_tick;
      EventType m_eventType;
      LabelStr m_objectName;
      LabelStr m_predicateName;
    };

    /**
     * @brief The Agent is a singleton per process.
     */
    static AgentId initialize(const TiXmlElement& configData, Clock& clock, TICK timeLimit = 0);

    /**
     * @brief Accessor for the singleton instance
     */
    static const AgentId& instance();

    /**
     * @brief Allow a reset of the current instance - deallocate it.
     */
    static void reset();

    static void cleanupLog();

    /**
     * Returns the TICK value corresponding to 'forever'
     */
    static TICK forever();

    virtual ~Agent();

    /**
     * @brief Run the agent. Will block till it is done with its mission. Run it on a separate thread if you need to do more than just run the agent.
     */
    void run();

    /**
     * @brief Terminate the agent
     */
    static void terminate();

    /**
     * @brief Access termination flag
     */
    static bool terminated();

    /**
     * @brief Run the agent for a next cycle of execution. Allows for externally timing control in testing
     * @return true if not yet complete, otherwise false.
     */
    bool doNext();

    /**
     * @brief Accessor
     */
    const AgentId& getId() const;

    /**
     * @brief Accessor
     */
    const LabelStr& getName() const;

    /**
     * @brief Retrieve Reactor by name
     */
    const TeleoReactorId& getReactor(const LabelStr& name);

    /**
     * @brief Retrieve Onwer reactor for timeline
     */
    const TeleoReactorId& getOwner(const LabelStr& timeline);

    /**
     * @brief Get the reactor count
     */
    int getReactorCount() const {return m_reactors.size();}

    /**
     * @brief Called by Reactors when the post observations. The agent will route to 0 or more reactors who track this observation timeline.
     * @param observation The observation reported.
     * @see Observer
     */
    void notify(const Observation& observation);

    /**
     * @brief Call back to log a request being sent to a reactor
     */
    void logRequest(const TokenId& goal);

    /**
     * @brief Call back to log a recall being sent to a reactor
     */
    void logRecall(const TokenId& goal);

    /**
     * @brief Test if the mission is over. Exposed publically to facilitate testing
     */
    bool missionCompleted() const;

    /**
     * @brief Get the current clock value
     */
    TICK getCurrentTick() const;

    /**
     * @brief Retrieve the final tick for the agent. Used to limit the planning horizon
     */
    TICK getFinalTick() const;

    /**
     * @brief Accessor for the clock
     */
    const Clock& getClock() const;

    /**
     * Over-write default, allowing different statistics collector
     */
    void setMonitor(PerformanceMonitor& monitor);

    /**
     * @brief Accessor for a performance monitor
     */
    const PerformanceMonitor& getMonitor() const;

    /**
     * @brief Accessor for the event log
     */
    const std::vector<Event>& getEventLog() const;

    /**
     * @brief Utility to create an event log
     */
    static std::string toString(const  std::vector<Event>& eventLog);

    /**
     * @brief Constant for Timeline base class
     */
    static const LabelStr& TIMELINE();

    /**
     * @brief Constant for the Action base class
     */
    static const LabelStr& ACTION();

    /**
     * @brief Accessor for Mode constant
     */
    static const LabelStr& TIMELINE_MODE();

    /**
     * @brief Accessor for Internal Timeline
     */
    static const LabelStr& INTERNAL_TIMELINE();

    /**
     * @brief Accessor for External Timeline
     */
    static const LabelStr& EXTERNAL_TIMELINE();

    /**
     * @brief Accessor for Ignore Timeline
     */
    static const LabelStr& IGNORE_TIMELINE();


  private:

    static LabelStr buildLogName(LabelStr const &prefix);

    /**
     * @brief Instantiated by singleton initialization function
     */
    Agent(const TiXmlElement& configData, Clock& clock, TICK timelimit, bool enableLogging = true);

    /**
     * @brief execute the next reactor for a step.
     * @return true if more work to do
     */
    bool executeReactor();

    /**
     * @brief This is called just before we move to the next tick.
     */
    void handleTickEnd();

    /**
     * @brief Called at the start of a new tick. Dispatches commands
     */
    void handleTickStart();

    /**
     * @brief Called to synchronize values at the execution frontier across all reactors.
     */
    void synchronize();

    /**
     * @brief Select the next reactor to work on
     * @return reactor The next reactor to work on. If no work required, returns a noId()
     */
    TeleoReactorId nextReactor();

    /**
     * Helper method to obtain the correct final tick value from the input parameter string
     */
    static TICK getFinalTick(const char * valueStr);

    static AgentId s_id; /*!< Store for the singleton instance */
    AgentId m_id; /*!< This Id */
    const LabelStr m_name; /*! Name - from configuration file. */
    ObserverId m_thisObserver; /*!< A connector to allow the agent to play as a middleman by intercepting observations from Reactors */
    unsigned int m_currentTick; /*!< Set by the clock */
    unsigned int m_finalTick; /*!< Determines mission end */
    std::multimap<LabelStr, ObserverId> m_observersByTimeline; /*!< Routing table for observations */
    std::vector<TeleoReactorId> m_reactors; /*!< The reactors in order of allocation */
    std::map< double, TeleoReactorId> m_reactorsByName; /*!< The set of reactors */
    std::map< double, TeleoReactorId> m_ownersByTimeline; /*!< Lookup table for getting owners */
    std::vector<TeleoReactorId> m_sortedReactors; /*!< Sorted by dependency for synchronization */
    std::vector<TeleoReactorId> m_deliberators; /*!< The agenda of reactors for deliberation. Refreshed on every tick. */

    Clock& m_clock;
    PerformanceMonitor m_monitor;
    RStat m_synchUsage;
    RStat m_deliberationUsage;

    const bool m_enableEventLogger; /*!< If true, the agent will store events */
    std::vector<Event> m_eventLog; /*!< Used for analysis and testing */

    ObservationLogger m_obsLog;

    static bool s_terminated; /*!< Used to terminated */    
  };

}

#endif
