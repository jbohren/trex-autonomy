/* -*- C++ -*-
 * $Id$
 */
/** @file "ObserverReactor.cc"
 *
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#include "ObserverReactor.hh"
#include "StringExtract.hh"

#include "Token.hh"

using namespace TREX;

/*
 * class ObserverReactor
 */

// Statics :

// Just a copy/paste from Adapter.cc
TiXmlElement const &ObserverReactor::externalConfig(TiXmlElement const &sourceConfig) {
  static TiXmlElement *sl_xml = NULL;
  static std::string sl_fileName;

  std::string const newFile = extractData(sourceConfig, "config").c_str();
  
  if( NULL==sl_xml ) {
    sl_xml = LogManager::initXml(newFile);
    sl_fileName = newFile;
  }

  if( sl_fileName.find(newFile)!=0 ) {
    delete sl_xml;
    sl_xml = LogManager::initXml(newFile);
    sl_fileName = newFile;    
  }
  return *sl_xml;
}

// Just a copy/paste from Adapter.cc
void ObserverReactor::getTimelines(std::map<LabelStr, ServerId> &result, 
				 TiXmlElement const &configData) {
  TiXmlElement const &configSource = externalConfig(configData);

  for(TiXmlElement * child = configSource.FirstChildElement();
      NULL!=child; child = child->NextSiblingElement() ) {
    if( 0==strcmp(child->Value(), "Timeline") ) {
      ServerId noServer;
      LabelStr name = extractData(*child, "name");
      result.insert(std::make_pair(name, noServer));
    }
  }
}

// Structors :

ObserverReactor::ObserverReactor(LabelStr const &agentName, TiXmlElement const &configData)
  :TeleoReactor(agentName, configData) {
  debugMsg("ObserverReactor",nameString() << " Loading configuration.");
  getTimelines(m_externals, configData);
}

ObserverReactor::~ObserverReactor() {
  debugMsg("ObserverReactor:~ObserverReactor", " EOP");
}

// Handlers :

void ObserverReactor::handleInit(TICK initialTick, 
			       std::map<double, ServerId> const &serversByTimeline,
			       ObserverId const &observer) {
  debugMsg("ObserverReactor:handleInit",nameString() << " Connecting to servers.");
  for(std::map<LabelStr, ServerId>::iterator i = m_externals.begin(), 
	endi = m_externals.end(); endi!=i; ++i ) {
    std::map<double, ServerId>::const_iterator it =serversByTimeline.find(i->first);
    
    checkError(serversByTimeline.end()!=it, 
	       "Could not find a server for "<<i->first.toString());
    i->second = it->second; 
  }
}

void ObserverReactor::notify(Observation const &obs) {
  debugMsg("ObserverReactor:notify",nameString() << obs.toString() 
	   << " received at " << getCurrentTick());
}

void ObserverReactor::handleRequest(const TokenId& goal) {
  // Normally it will be never called
  debugMsg("ObserverReactor:handleRequest",nameString() << " Request received for " 
	   << goal->toString());
}

void ObserverReactor::handleRecall(const TokenId& goal) {
  // Normally it will be never called
  debugMsg("ObserverReactor:handleRecall",nameString() << " Recall received for " 
	   << goal->toString());
}

// Observers :

void ObserverReactor::queryTimelineModes(std::list<LabelStr> &externals,
				       std::list<LabelStr> &internals) {
  for(std::map<LabelStr, ServerId>::const_iterator i = m_externals.begin(),
	endi=m_externals.end(); endi!=i; ++i)
    externals.push_back(i->first);
}

std::string ObserverReactor::nameString() const {
    std::ostringstream oss;
    oss << "[" << getName().toString() << "][" << getCurrentTick() << "]";
    return oss.str();
}
