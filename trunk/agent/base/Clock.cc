#include "Clock.hh"
#include <errno.h>
#include <time.h>
#include <cmath>
#include <cstring>

#include "LogManager.hh"
#include "Guardian.hh"

namespace TREX {

  void Clock::doStart() {
    TickLogger *clk = LogManager::instance().getTickLog(CPU_STAT_LOG);
    clk->addField("userTime", m_diff.user_time());
    clk->addField("systemTime", m_diff.system_time());
    clk->addField("maxRSS", m_cur.max_resident());
    start();
  }

  void Clock::sleep(double sleepDuration){
    if( sleepDuration>0.0 ) {
      struct timespec tv;
      tv.tv_sec = (time_t) sleepDuration;
      tv.tv_nsec = (long) ((sleepDuration - tv.tv_sec) * 1e+9);
      
      while( tv.tv_sec>0 || tv.tv_nsec>0 ){
	int rval = nanosleep(&tv, &tv);
	
	if(rval == 0) // We are done sleeping
	  return;
	
	checkError(errno == EINTR, "Must be interrupted but wasn't. Errno is: " << errno);
      }
    }
  }

  void Clock::advanceTick(TICK &tick) {
    if( m_processStats ) {
      try {
	RStat tmp(m_cur);
	m_cur.reset(RStat::self);
	m_diff = m_cur-tmp;
      } catch(ErrnoExcept e) {
	
	debugMsg("TREX", e.what() << "Disabling process statistics.");
	m_processStats = false;
      }
    }
    ++tick;
  }

  /**
   * @brief Uses a high resolution sleep method internally
   */
  void Clock::sleep() const {
    sleep(getSleepDelay());
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
      m_started(false), m_tick(0), m_floatTick(secondsPerTick) {
    m_secondsPerTick.tv_sec = static_cast<long>(std::floor(secondsPerTick));
    m_secondsPerTick.tv_usec = static_cast<long>(std::floor((secondsPerTick-m_secondsPerTick.tv_sec)*1e6));
  }

  void RealTimeClock::start(){
    getDate(m_nextTickDate);
    setNextTickDate();
    m_started = true;
  }

  void RealTimeClock::setNextTickDate(unsigned factor) {
    m_nextTickDate.tv_usec += factor*m_secondsPerTick.tv_usec;
    m_nextTickDate.tv_sec += factor*m_secondsPerTick.tv_sec + (m_nextTickDate.tv_usec/1000000);
    m_nextTickDate.tv_usec %= 1000000;
  }

  double RealTimeClock::timeLeft() const {
    timeval tv;
    double result;
    getDate(tv);
    
    result = m_nextTickDate.tv_usec - tv.tv_usec;
    result /= 1e6;
    result += m_nextTickDate.tv_sec - tv.tv_sec;

    return result;
  }
    
  TICK RealTimeClock::getNextTick(){
   Guardian<Mutex> guard(m_lock);
    
    if( m_started ) {      
      double howLate = -timeLeft();
      
      if( howLate>=0 ) {
	int tickIncr = 1+(int) std::floor(howLate/m_floatTick);
	m_tick += tickIncr;
	setNextTickDate(tickIncr);
// 	TREXLog()<<"[clock]["<<m_tick<<"] "<<howLate<<" secs late."<<std::endl; 
      }
    }
    return m_tick;
  }

  double RealTimeClock::getSleepDelay() const {    
    if( m_started ) {
      double delay;
      {
	Guardian<Mutex> guard(m_lock);
	delay = timeLeft();
      }
//       TREXLog()<<"[clock]["<<m_tick<<"] sleeping for "<<delay<<" secs."<<std::endl;
      return delay;
    } else 
      return Clock::getSleepDelay();
  }

}
