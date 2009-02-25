#include "Adapter.hh"
#include "Observer.hh"
#include "Object.hh"
#include "Debug.hh"
#include "Token.hh"
#include "TokenVariable.hh"
#include "Observer.hh"

/**
 * @author Conor McGann
 * @brief Demonstrate plug-in of handling goal requests and generating observations
 * Used to test the recall function where it will generate notifications for a single numeric timeline based on the tick value. This will
 * conflict with the planned values for the other reactors.
 */
namespace TREX {

  class RecallAdapter: public Adapter {
  public:
    RecallAdapter(const LabelStr& agentName, const TiXmlElement& configData)
      : Adapter(agentName, configData){}

    void handleInit(TICK initialTick, const std::map<double, ServerId>& serversByTimeline, const ObserverId& observer){
      m_observer = observer;
      ObservationByValue obs("c", "NumberTimeline.holds");
      obs.push_back("value", new IntervalIntDomain(0,0));
      m_observer->notify(obs);
    }

    /**
     * Force observation of value based on tick value. This will force plan failures directly and indirectly.
     */
    bool synchronize(){
      ObservationByValue obs("c", "NumberTimeline.holds");
      obs.push_back("value", new IntervalIntDomain(getCurrentTick(),getCurrentTick()));
      m_observer->notify(obs);
      return true;
    }

    void notify(const Observation& observation){
      checkError("This should never happen", observation.getObjectName().toString());
    }

    bool handleRequest(const TokenId& goal){ return true;}

  private:
    ObserverId m_observer; /*!< The agent observer interface*/
  };

  // Allocate a Factory
  TeleoReactor::ConcreteFactory<RecallAdapter> l_RecallAdapter_Factory("RecallAdapter");
}
