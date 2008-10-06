#ifndef H_Adapter
#define H_Adapter

/**
 * @author Conor McGann
 * @file Provides declaration of Adapter which is a TeleoReactor used to integrate external control components
 */
#include <set>

#include "TeleoReactor.hh"

namespace TREX {

  class Adapter: public TeleoReactor {
  public:
    /**
     * @brief Simple constructor.
     * @param agentName the name of the agent in which it is to be included.
     * @param configData the configuration xml element to set up connection. Extendable from base data elements.
     * @param logDefault logging flag value in the absence of the log attribute
     */
    Adapter(const LabelStr& agentName, const TiXmlElement& configData, bool logDefault =false);

    /**
     * @brief Constructor
     * @param agentName The name of the agent in which it belongs.
     * @param name The name of the reactor
     * @param lookead The lookahead
     * @param latency The latency
     * @param configSrc The configuration file to configure timelines
     * @param logDefault logging flag value in the absence of the log attribute
     */
    Adapter(const LabelStr& agentName, const LabelStr& name, TICK lookAhead, TICK latency, const LabelStr& configSrc,
	    bool logDefault=false);

    /**
     * @brief Constructor for use when config data in the derived class is standardized to avoid unnecessary verbiage in the
     * configuration file
     * @param agentName the name of the agent in which it is to be included.
     * @param configData the configuration xml element to set up connection. Extendable from base data elements.
     * @param lookead The lookahead
     * @param latency The latency
     * @param logDefault logging flag value in the absence of the log attribute
     */
    Adapter(const LabelStr& agentName, const TiXmlElement& configData, TICK lookAhead, TICK latency, bool logDefault =false);

    virtual ~Adapter();

    /**
     * @brief Provide internals and clients only. That is, it can be the source for observations and the sink for goals
     * @param externals Append to this list any timeline names for which the adapter wishes to subscribe to updates.
     * @param internals Append to this list any timeline names for which this adapter is the owner. It can receive requests and publish
     * observations for values on this timeline.
     */
    void queryTimelineModes(std::list<LabelStr>& externals, std::list<LabelStr>& internals);

    void handleInit(TICK initialTick, const std::map<double, ServerId>& serversByTimeline, const ObserverId& observer);

    /**
     * Helper method to re-use code to populate timelines
     */
    static void getTimelines(std::set<LabelStr>& results, const TiXmlElement& configData);

    static const TiXmlElement& externalConfig(const TiXmlElement& sourceConfig);

    static const TiXmlElement& getConfig(const LabelStr& configFile);
  protected:
    /* STUBS since adapter should handle immediately */
    bool hasWork() {return false;}
    void resume(){}
    bool sendNotify(Observation const &obs);

  private:
    ObserverId m_observer;
    std::set<LabelStr> m_internals; /*!< The timelines it will accept goals on and issue observations */
  };
}
#endif
