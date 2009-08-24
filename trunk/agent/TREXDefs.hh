#include "Id.hh"
#include "LabelStr.hh"
#include "Debug.hh"

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

/**
  @brief Create a logging message, which will
  only be created or used when the given condition is true at run time.
  @param marker A string that "marks" the message to enable it by.
  @param data The data to be printed when the message is enabled.
*/
#define TREX_INFO(marker, data) TREX_INFO_COND(true, marker, data)

/**
  @brief Create a conditional logging message, which will
  only be created or used when the given condition is true at run time.
  @param cond An additional condition to be checked before printing the message,
         which can be any C/C++ expression that could be used in an if statement.
  @param marker A string that "marks" the message to enable it by.
  @param data The data to be printed when the message is enabled.
*/
#define TREX_INFO_COND(cond, marker, data) { \
  static DebugMessage *dmPtr = DebugMessage::addMsg(__FILE__, __LINE__, marker); \
  if (dmPtr->isEnabled() && (cond)) { \
    try { \
      getStream().exceptions(std::ios_base::badbit); \
      getStream() << "[" << marker << "]" << data << std::endl; \
    } \
    catch(std::ios_base::failure& exc) { \
      checkError(ALWAYS_FAIL, exc.what()); \
      throw; \
    } \
  } \
}
