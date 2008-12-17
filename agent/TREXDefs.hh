#include "Id.hh"
#include "LabelStr.hh"


// As a sanity check we can switch to using the coed generator
//#define USE_CODE_GENERATOR

using namespace EUROPA;

namespace TREX {

  /*!< The type for a tick - a unit of time */
  typedef unsigned int TICK;

  class Agent;
  typedef Id<Agent> AgentId;

  class AgentListener;
  typedef Id<AgentListener> AgentListenerId;

  class TeleoReactor;
  typedef Id<TeleoReactor> TeleoReactorId;

  class Server;
  typedef Id<Server> ServerId;

  class Observer;
  typedef Id<Observer> ObserverId;

  class Observation;

  class DbCore;
  typedef Id<DbCore> DbCoreId;

  /**
   * @brief Module initialization function
   */
  void initTREX();

  /**
   * @brief Model cleanup function
   */
  void uninitTREX();

  /* USEFUL UTILITIES */

  LabelStr compose(const LabelStr& prefix, const LabelStr& suffix);
}

#define CPU_STAT_LOG "cpuStat.log"
