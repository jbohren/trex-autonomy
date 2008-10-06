#ifndef H_TeleoReactor
#define H_TeleoReactor

/**
 * @author Conor McGann
 * @file Declares the main interface for a Teleo-Reactor
 */

#include "TREXDefs.hh"
#include "XMLUtils.hh"
#include "Observer.hh"
#include "LogManager.hh"
#include "RStat.hh"

#include <list>
#include <map>

namespace TREX {

  class TeleoReactor {
  public:

    virtual ~TeleoReactor();

    const TeleoReactorId& getId() const;

    const LabelStr& getName() const;

    const LabelStr& getAgentName() const;

    TICK getCurrentTick() const;

    /**
     * @brief Helper method for debug output: "[name(tick)]"
     */
    std::string nameString() const;

    /**
     * @brief Get priority based on dependency level in a DAG
     */
    int getPriority(int callCount = 0);

    static void sort(std::vector<TeleoReactorId>& reactors);

    /**
     * @brief Return a narrower interface for observation only.
     */
    const ObserverId& toObserver() const {return m_thisObserver;}

    /**
     * @brief return a narrower interface for dispatch only
     */
    const ServerId& toServer() const {return m_thisServer;}

    /**
     * @brief Interception for requests received so they can be logged prior to delegation
     * @param goal The goal token.
     * @return true if the request has been received and does not need to be re-published. False if the request
     * should be republished.
     */
    bool request(const TokenId& goal);

    /**
     * @brief Interception for recalls received so they can be logged prior to delegation
     */
    void recall(const TokenId& goal);

    /**
     * @brief Handle observations.
     */
    virtual void notify(const Observation& observation);

    /**
     * @brief Commands the server to handle a request expressed as a goal network.
     * @param goal The goal token.
     * @return true if the request has been received and does not need to be re-published. False if the request
     * should be republished.
     */
    virtual bool handleRequest(const TokenId& goal);

    /**
     * @brief Tells the server to handle the recall notification for the given goals
     */
    virtual void handleRecall(const TokenId& goal);

    /**
     * @brief Retrieve the latency it takes to respond. This is used in planning when to dispatch goals. It is a lower bound on dispatch window.
     */
    TICK getLatency() const;

    /**
     * @brief Retrieve the look-ahead window. This is used in deciding how far ahead to commit goals. It is an upper bound on the dispatch window.
     */
    TICK getLookAhead() const;

    /**
     * @brief Retrieves the timelines according to each mode.
     * @param externals Results for all timelines which are externally owned. Will dispatch goals and synchronize with observations.
     * @param internals Results for all timelines which are internally owned. Will publish observations at the execution frontier.
     */
    virtual void queryTimelineModes(std::list<LabelStr>& externals,
				    std::list<LabelStr>& internals) = 0;

    bool shouldLog() const {
      return m_shouldLog;
    }

    static TeleoReactorId createInstance(const LabelStr& agentName, const LabelStr& component, const TiXmlElement& configData);

    /**
     * @brief Inner classes to allow allocation of Teleo Reactors from configuration file
     */
    class Factory {
    public:
      Factory(const LabelStr& name);

      virtual ~Factory();

      const Id<Factory>& getId() const;

      /**
       * @brief Implemented by concrete sub-classes
       */
      virtual TeleoReactorId  createInstance(const LabelStr& agentName, const TiXmlElement& configData) = 0;

    private:
      Id<Factory> m_id;
      const LabelStr& m_name;
    };

    template<class T>
    class ConcreteFactory : public Factory {
    public:
      ConcreteFactory(const LabelStr& name): Factory(name) {}
 
      TeleoReactorId  createInstance(const LabelStr& agentName, const TiXmlElement& configData){
	return (new T(agentName, configData))->getId();
      }
    };

    /**
     * @brief Registers a TeleoReactor factory
     * @param name The name of the component factory
     * @param factory The factory to use for allocation
     */
    static void registerFactory(const LabelStr& name, const Id<TeleoReactor::Factory>& factory);

    static void purgeAll();

    /**
     * @brief USed to hook up observer for dispatch of observations and servers for dispatch of goals
     */
    void doHandleInit(TICK initialTick, std::map<double, ServerId> const &serversByTimeline, 
		      ObserverId const &observer);

    /**
     * @brief Next tick notification.
     *
     * This method is called by Agent to notify the reactor that a new tick has started.
     *
     * @see void handleTickStart()
     */
    void doHandleTickStart();

    /**
     * @brief Synchronize buffered observations.
     */
    bool doSynchronize();

    /**
     * @brief Test if the reactor has any work to do
     */
    virtual bool hasWork() = 0;

    void doResume();


  protected:
    /**
     * @brief Accessor for lookup table to handle factory classes.
     */
    static std::map<double, Id<Factory> >& lookupTable();

    /**
     * @brief Internal method to load the factory if available.
     * @return A good id if found, otherwise a noId()
     */
    static Id<TeleoReactor::Factory> getFactory(const LabelStr& name);

    /**
     * @brief USed to hook up observer for dispatch of observations and servers for dispatch of goals
     */
    virtual void handleInit(TICK initialTick, const std::map<double, ServerId>& serversByTimeline, const ObserverId& observer){}

    /**
     * @brief Implement this when the next tick occurs.
     */
    virtual void handleTickStart(){}

    /**
     * @brief Synchronize buffered observations.
     */
    virtual bool synchronize(){return true;}

    /**
     * @brief resume the work of the reactor.
     */
    virtual void resume() = 0;

    /**
     * @brief Constructor will set the timing parameters
     * @param configData xml configuration element.
     * @param logDefault default logging flag value in the absence
     * of the log attribute 
     */
    TeleoReactor(const LabelStr& agentName, const TiXmlElement& configData, bool logDefault=false);

    /**
     * @brief Allow construction where inputs are provided as arguments instead of via a config file.
     * @param agentName The name of the agent in which it belongs.
     * @param name The name of the reactor
     * @param lookead The lookahead
     * @param latency The latency
     * @param log The logging flag 
     */
    TeleoReactor(const LabelStr& agentName, const LabelStr& name, TICK lookAhead, TICK latency, bool log=false);


    /**
     * @brief Constructor will be explicitly passed the timing parameters
     * @param configData xml configuration element.
     * @param lookead The lookahead
     * @param latency The latency
     * @param logDefault default logging flag value in the absence of the log attribute 
     */
    TeleoReactor(const LabelStr& agentName, const TiXmlElement& configData, TICK lookAhead, TICK latency, bool logDefault=false);

  private:
    static TICK getLookAheadFromXML(const TiXmlElement& configData);
    TeleoReactorId m_id;
    const LabelStr m_name;
    const LabelStr m_agentName;
    const TICK m_lookAhead, m_latency;
    ObserverId m_thisObserver; /*!< A narrower interface to receive observations */
    ServerId m_thisServer; /*!< A narrower interface to receive goal requests */

    size_t m_syncCount, m_searchCount;
    RStat m_syncUsage, m_searchUsage;

    bool const m_shouldLog;

  };

}

#endif
