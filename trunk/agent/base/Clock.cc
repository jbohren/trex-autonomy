#include "Clock.hh"
#include <errno.h>
#include <time.h>
#include <cmath>

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
    tv.tv_sec = (time_t) sleepDuration;
    tv.tv_nsec = (long) ((sleepDuration - tv.tv_sec) * 1e+9);

    while( tv.tv_sec>0 || tv.tv_nsec>0 ){
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

  TICK PseudoClock::selectStep(unsigned int stepsPerTick) {
    if( stepsPerTick<=0 ) {
      TREXLog()<<"requested number of steps is invalid ("<<stepsPerTick<<")."
	" Setting it to 50."<<std::endl;
      stepsPerTick = 50;
    }
    return stepsPerTick;
  }

  PseudoClock::PseudoClock(double sleepSeconds, unsigned int stepsPerTick, bool stats) : 
    Clock(sleepSeconds, stats), m_tick(0), m_internalTicks(0), m_stepsPerTick(selectStep(stepsPerTick)) {
  }
 
  TICK PseudoClock::getNextTick() {
    if(m_internalTicks > 0 && ((m_internalTicks % m_stepsPerTick) == 0))
      Clock::advanceTick(m_tick);
    m_internalTicks++;
    return m_tick ;
  }

  /**
   * Real Time Clock
   */
  void RealTimeClock::getDate(timeval &date) {
    gettimeofday(&date, NULL);
  }

  RealTimeClock::RealTimeClock(double secondsPerTick, bool stats)
    : Clock(secondsPerTick/1000, stats),
      m_started(false), m_tick(0) {
    m_secondsPerTick.tv_sec = static_cast<long>(std::floor(secondsPerTick));
    m_secondsPerTick.tv_usec = static_cast<long>(std::floor((secondsPerTick-m_secondsPerTick.tv_sec)*1e6));
    pthread_mutex_init(&m_lock, NULL);
  }

  void RealTimeClock::start(){
    getDate(m_nextTickDate);
    setNextTickDate();
    m_started = true;
    pthread_create(&m_thread, NULL, threadRunner, this);
  }

  void RealTimeClock::setNextTickDate() {
    m_nextTickDate.tv_usec += m_secondsPerTick.tv_usec;
    m_nextTickDate.tv_sec += m_secondsPerTick.tv_sec + (m_nextTickDate.tv_usec/1000000);
    m_nextTickDate.tv_usec %= 1000000;
  }

  double RealTimeClock::timeToNextTick() const {
    timeval tv;
    double result;
    getDate(tv);
    
    result = m_nextTickDate.tv_usec - tv.tv_usec;
    result /= 1e6;
    result += m_nextTickDate.tv_sec-tv.tv_sec;
    return result;
  }
    
  TICK RealTimeClock::getNextTick(){
    pthread_mutex_lock(&m_lock);
    TICK tick = m_tick;
    pthread_mutex_unlock(&m_lock);
    return tick;
  }

  void RealTimeClock::sleep() const {
    if( m_started ) {
      double delay;
    
      pthread_mutex_lock(&m_lock);
      delay = timeToNextTick();
      pthread_mutex_unlock(&m_lock);
      
      if( delay>0.0 )
	Clock::sleep(delay);
    } else 
      Clock::sleep();
  }

  void* RealTimeClock::threadRunner(void* clk){
    RealTimeClock* This = (RealTimeClock*) clk;

    // Loop forever, sleep for a tick
    while(true){
      Clock::sleep(This->timeToNextTick());
      pthread_mutex_lock(&This->m_lock);
      This->advanceTick(This->m_tick);
      This->setNextTickDate();
      pthread_mutex_unlock(&This->m_lock);
    }

    return NULL;
  }
}

