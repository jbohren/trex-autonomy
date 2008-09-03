/**
 * @author Conor McGann
 */
#include "Agent.hh"
#include "Adapter.hh"
#include "StringExtract.hh"

namespace TREX {

  Adapter::Adapter(const LabelStr& agentName, const TiXmlElement& configData, bool logDefault)
    : TeleoReactor(agentName, configData, 
		   string_cast<bool>(logDefault, 
				     checked_string(externalConfig(configData).Attribute("log")))) {
    getTimelines(m_internals, externalConfig(configData));
  }

  Adapter::Adapter(const LabelStr& agentName, const LabelStr& name, TICK lookAhead, TICK latency, const LabelStr& configFile, bool logDefault)
    : TeleoReactor(agentName, name, lookAhead, latency, logDefault) {
    getTimelines(m_internals, getConfig(configFile));
  }

  Adapter::~Adapter(){}

  void Adapter::getTimelines(std::set<LabelStr>& results, const TiXmlElement& configSource){
    // Iterate over internal and external configuration specifications
    for (TiXmlElement * child = configSource.FirstChildElement();
           child != NULL;
           child = child->NextSiblingElement()) {

        if(strcmp(child->Value(), "Timeline") == 0) {
	  LabelStr name = extractData(*child, "name");
	  results.insert(name);
	}
    }
  }

  void Adapter::queryTimelineModes(std::list<LabelStr>& externals, std::list<LabelStr>& internals){
    checkError(!m_internals.empty(), "Adapter configuration error for " << getName().toString() << ". There must be at least one timeline.");

    // Add the standard elements for an adapter
    internals.assign(m_internals.begin(), m_internals.end());
  }

  void Adapter::handleInit(TICK initialTick, const std::map<double, ServerId>& serversByTimeline, 
			   const ObserverId& observer) {
    m_observer = observer;
  }

  bool Adapter::sendNotify(Observation const &obs) {
    if( m_internals.find(obs.getObjectName())!=m_internals.end() ) {
      checkError( m_observer.isValid(), "Adapter error : unable to notify (Adapter::handleInit was not called)");
      m_observer->notify(obs);
      return true;
    }
    return false;
  }

  const TiXmlElement& Adapter::externalConfig( const TiXmlElement& configSrc){
    if(configSrc.Attribute("config") != NULL){
      const std::string newFile =  extractData(configSrc, "config").c_str();
      return getConfig(newFile);
    }

    return configSrc;
  }

  const TiXmlElement& Adapter::getConfig(const LabelStr& configFile){
    static TiXmlElement* sl_xml = 0;
    static LabelStr sl_fileName;

    // If no element, allocate
    if(sl_xml == NULL){
      sl_xml = LogManager::initXml(configFile.toString());
      sl_fileName = configFile;
    }

    // If wrong element, delete and allocate
    if(!(sl_fileName == configFile)){
      delete sl_xml;
      sl_xml = LogManager::initXml(configFile.toString());
      sl_fileName = configFile;
    }

    return *sl_xml;
  }

}
