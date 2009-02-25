/* -*- C++ -*-
 * $Id$
 */
/** @file "ObservationLogger.cc"
 *
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#include <ctime>
#include <list>

#include "Utils.hh"

#include "LogManager.hh"
#include "ObservationLogger.hh"
#include "Agent.hh"

using namespace TREX;

/*
 * class ObservationLogger
 */

// Structors :

ObservationLogger::ObservationLogger(LabelStr const &logName)
  :m_inHeader(true), m_empty(true), 
   m_logName(LogManager::instance().file_name(logName.toString())), 
   m_logFile(NULL), m_hasData(false) { 
}

ObservationLogger::~ObservationLogger() {
  endFile();
}

// Modifiers :

void ObservationLogger::startFile() {
  
  time_t cur_date;
  debugMsg("ObsLog", "Opening file "<<m_logName.toString());
  m_logFile = fopen(m_logName.c_str(), "w+");
  
    
  time(&cur_date);
  char *str_date = ctime(&cur_date);
  str_date[strlen(str_date)-1] = '\0';
    
  fprintf(m_logFile, "<?xml version=\"1.0\" standalone=\"no\"?>\n\n"
	  "<Log date=\"%s\">\n"
	  "\t<Declare>\n", str_date); 
}

void ObservationLogger::endFile() {
  if( NULL!=m_logFile ) {
    if( m_hasData ) 
      fprintf(m_logFile, "\t</Tick>\n");
    fprintf(m_logFile, "</Log>\n");
    fclose(m_logFile);
    m_logFile = NULL;
  }
}

void ObservationLogger::declTimeline(LabelStr const &name, 
				     LabelStr const &owner) {
  checkError(m_inHeader, 
	     "ObservationLogger: Timeline declaration allowed only in header.");
  std::map<LabelStr, LabelStr>::value_type to_ins(name, owner);
  debugMsg("ObsLog:declTimeline", owner.toString()<<" ["<<name.toString()<<"]");
  
  if( !m_timelines.insert(to_ins).second ) {
    std::ostringstream oss;
    
    oss<<"ObservationLogger : timeline \""<<name<<"\" multiply declared.\n"
       <<"                    keeping the first one.";
    debugMsg("TREX", oss.str());
  }
}

void ObservationLogger::endHeader(TICK init) {
  if( m_inHeader && !m_timelines.empty() ) {
    std::map< LabelStr, std::list<LabelStr> > decls;
    std::map< LabelStr, LabelStr >::const_iterator i, endi = m_timelines.end();
    
    for( i=m_timelines.begin(); endi!=i; ++i ) 
      decls[i->second].push_back(i->first);
    
    std::map< LabelStr, std::list<LabelStr> >::iterator j, 
      endj = decls.end();
    
    startFile();

    for( j = decls.begin(); endj!=j ; ++j ) {
      fprintf(m_logFile, "\t\t<Adapter name=\"%s\">\n",j->first.c_str());
      while( !j->second.empty() ) {
	fprintf(m_logFile, "\t\t\t<Timeline name=\"%s\"/>\n",
		j->second.front().c_str());
	j->second.pop_front();
      }
      fprintf(m_logFile,"\t\t</Adapter>\n");
    }
    fprintf(m_logFile,"\t</Declare>\n");
//     long pos = ftell(m_logFile);
//     fprintf(m_logFile, "</Log>\n");
//     fseek(m_logFile, pos, SEEK_SET);
  }
  m_inHeader = false;
  m_lastTick = init;
}

void ObservationLogger::log(Observation const &obs) {
  checkError(!m_inHeader, "ObservationLogger : new observation while in header.");
  
  if( NULL!=m_logFile ) {
    LabelStr timeLine = obs.getObjectName();
    if( m_timelines.find(timeLine)!=m_timelines.end() ) {
      debugMsg("ObsLog:log", obs.toString());
      if ( m_lastTick!=Agent::instance()->getCurrentTick() || m_empty ) {
	m_lastTick = Agent::instance()->getCurrentTick();
	if ( m_hasData ) {
	  fprintf(m_logFile, "\t</Tick>\n");
	}
	fprintf(m_logFile, "\t<Tick value=\"%u\">\n", m_lastTick);
	m_hasData = true;
      }
      m_empty = false;
      obs.printXML(m_logFile);
      fprintf(m_logFile, "\n"); 
    }
  }
}
