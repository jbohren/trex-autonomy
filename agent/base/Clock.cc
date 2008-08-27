#include "Clock.hh"
#include <errno.h>
#include <time.h>

#include "LogManager.hh"

namespace TREX {

  void Clock::doStart() {
    TickLogger *clk = LogManager::instance().getTickLog(CPU_STAT_LOG);
    clk->addField("userTime", m_diff.user_time());
    clk->addField("systemTime", m_diff.system_time());
    clk->addField("maxRSS", m_cur.max_resident());
    start();
  }

  void Clock::sleep(double sleepDuration){
    struct timespec tv;
    tv.tv_sec = (time_t) sleepDuration;;
    tv.tv_nsec = (long) ((sleepDuration - tv.tv_sec) * 1e+9);

    while(true){
	int rval = nanosleep(&tv, &tv);

	if(rval == 0) // We are done sleeping
	  return;

	checkError(errno == EINTR, "Must be interrupted but wasn't. Errno is: " << errno)
    }
  }

  void Clock::advanceTick(TICK &tick) {
    if( m_processStats ) {
      try {
	RStat tmp(m_cur);
	m_cur.reset();
	m_diff = m_cur-tmp;
      } catch(ErrnoExcept e) {
	warn(e.what());
	warn("\tDisabling process statisitics.");
	m_processStats = false;
      }
    }
    ++tick;
  }

  /**
   * @brief Uses a high resolution sleep method internally
   */
  void Clock::sleep() const {
    sleep(m_sleepSeconds);
  }

  PseudoClock::PseudoClock(double sleepSeconds, unsigned int stepsPerTick, bool stats) : 
    Clock(sleepSeconds, stats), m_tick(0), m_internalTicks(0), m_stepsPerTick(stepsPerTick) {}
 
  TICK PseudoClock::getNextTick() {
    if(m_internalTicks > 0 && ((m_internalTicks % m_stepsPerTick) == 0))
      Clock::advanceTick(m_tick);
    m_internalTicks++;
    return m_tick ;
  }

  /**
   * Real Time Clock
   */
  RealTimeClock::RealTimeClock(double secondsPerTick, bool stats)
    : Clock(secondsPerTick/1000, stats),
      m_tick(0),
      m_secondsPerTick(secondsPerTick) {
    pthread_mutex_init(&m_lock, NULL);
  }

  void RealTimeClock::start(){
    pthread_create(&m_thread, NULL, threadRunner, this);
  }

  TICK RealTimeClock::getNextTick(){
    pthread_mutex_lock(&m_lock);
    TICK tick = m_tick;
    pthread_mutex_unlock(&m_lock);
    return tick;
  }

  void* RealTimeClock::threadRunner(void* clk){
    RealTimeClock* This = (RealTimeClock*) clk;

    // Loop forever, sleep for a tick
    double sleepTime = This->m_secondsPerTick;
    while(true){
      Clock::sleep(sleepTime);
      pthread_mutex_lock(&This->m_lock);
      This->advanceTick(This->m_tick);
      pthread_mutex_unlock(&This->m_lock);
    }

    return NULL;
  }
}
