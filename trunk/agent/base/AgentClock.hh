#ifndef H_AgentClock
#define H_AgentClock

#include "TREXDefs.hh"
#include "TeleoReactor.hh"
#include "RStat.hh"
#include <sys/time.h>
#include "Mutex.hh"

/**
 * @brief Declaration of clock interface and implementation sub-classes
 */


namespace TREX {

  /**
   * @brief Abstract class to plug in different clocks
   */
  class Clock {
  public:
    virtual ~Clock(){}

    void doStart();

    /**
     * @brief Used by Agent to get the value of the next tick for synchronization
     */
    virtual TICK getNextTick() = 0;

    /**
     * @brief Helper method to provide a high-resolution sleep method
     * @see sleep(sleepDuration)
     */
    virtual void sleep() const;

    /**
     * @brief Utility to implement high-resolution sleep
     * @param sleepDuration The sleep duration in seconds. Accurate up to nanoseconds.
     */
    static void sleep(double sleepDuration);

    bool debugStats() const {
      return m_processStats;
    }
    
    /** @brief Global process statistics.
     *
     * @return the stats collected for the whole duration of the process.
     */
    RStat const &totalStat() const {
      return m_cur;
    }
    /** @brief Last tick process statistics.
     *
     * @return the stats of the process during the lasst tick.
     */
    RStat const &lastTickStat() const {
      return m_diff;
    }

    /**
     * @brief Accessor for seconds per tick
     */
    double getSecondsPerTick() const {
      return m_secondsPerTick;
    }

  protected:
    virtual double getSleepDelay() const {
      return 0;
    }
    /**
     * @brief Constructor
     * @param secondsPerTick The period of a single tick
     */
    Clock(double secondsPerTick, bool stats = true) : 
      m_secondsPerTick(secondsPerTick),
      m_processStats(stats)
    { }

    /**
     * @brief Called to start the clock counting
     */
    virtual void start(){}

    /** @brief Advance tick and update the stats.
     *
     * @param tick The main tick for clock
     *
     * Increment @e tick and update the process statisitcs for the last tick.
     *
     * @note This function has to be used only for the main clock tick.
     */
    void advanceTick(TICK &tick); 
    
    double m_secondsPerTick;

  private:
    bool m_processStats;
    RStat m_diff, m_cur;
  };

  /**
   * @brief Simple clock for stepping the code on the main thread
   */
  class PseudoClock: public Clock {
  public:
    PseudoClock(double secondsPerTick, unsigned int stepsPerTick, bool stats = true);

    /**
     * @brief Simple clock for stepping the code on the main thread
     */
    TICK getNextTick();
    virtual double getSleepDelay() const;

  private:
    /**
     * @brief Enforces a stepsPerTick > 0, writes to log if <0, and sets to some non-zero value
     */
    static TICK selectStep(unsigned int stepsPerTick);

    TICK m_tick;
    TICK m_internalTicks;
    const TICK m_stepsPerTick;
  };

  /**
   * @brief A clock that monitors time on a separate thread and generates updates to the tick.
   */
  class RealTimeClock: public Clock {
  public:
    RealTimeClock(double secondsPerTick, bool stats = true);

    /**
     * @brief Will idle till this is called.
     */
    void start();

    /**
     * @brief Retrieve the tick
     */
    TICK getNextTick();

  protected:
    double getSleepDelay() const;

  private:
    static void getDate(timeval &val);
    void setNextTickDate(unsigned factor=1);
    double timeLeft() const;

    bool m_started;
    TICK m_tick;
    timeval m_tvSecondsPerTick;
    timeval m_nextTickDate;
    mutable Mutex m_lock;
  };
    
}

#endif
