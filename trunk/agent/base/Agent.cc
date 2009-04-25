/**
 * @author Conor McGann
 * @file Implements singleton Agent
 */

#include "Agent.hh"
#include "Assembly.hh"
#include "Utils.hh"
#include "Utilities.hh"
#include "Observer.hh"
#include "Token.hh"
#include "TokenVariable.hh"
#include "Object.hh"
#include "LogManager.hh"
#include "AgentListener.hh"
#include <algorithm>

namespace TREX {

  AgentId Agent::s_id;

  bool Agent::s_terminated(false);

  TICK Agent::forever() {
    return PLUS_INFINITY / 8;
  }

  TICK Agent::getFinalTick(const char * valueStr){
    if(strcmp(valueStr, "forever") == 0)
      return  forever();

    return atoi(valueStr);
  }

  /**
   * @brief Connector to allow the agent to route observations. Will attach to internal reactors
   */
  class AgentObserver: public Observer {
  public:
    AgentObserver(const AgentId& agent): Observer(), m_agent(agent) {}

    void notify(const Observation& observation){
      m_agent->notify(observation);
    }

  private:
    AgentId m_agent;
  };

  AgentId Agent::initialize(const TiXmlElement& configData, Clock& clock, TICK timeLimit){
    checkError(s_id.isNoId(), "Already have an active agent. Must reset first.");
    new Agent(configData, clock, timeLimit);
    return s_id;
  }

  const AgentId& Agent::instance(){
    return s_id;
  }

  void Agent::reset(){
    checkError(s_id.isNoId() || s_id.isValid(), "Bad Agent Id.");

    if(s_id.isId())
      delete (Agent*) s_id;

    s_id = AgentId::noId();
  }

  void Agent::cleanupLog() {
    if( s_terminated && s_id.isId() ) {
      s_id->m_obsLog.endFile();
    }
  }

  Agent::Agent(const TiXmlElement& configData, Clock& clock, TICK timeLimit, bool enableLogging): 
    m_id(this), 
    m_name(extractData(configData, "name")),
    m_thisObserver(new AgentObserver(m_id)),
    m_currentTick(0),
    m_finalTick(timeLimit == 0 ?getFinalTick(extractData(configData, "finalTick").c_str()) : timeLimit),
    m_clock(clock),
    m_synchUsage(RStat::zeroed), 
    m_deliberationUsage(RStat::zeroed),
    m_enableEventLogger(enableLogging),
    m_obsLog(buildLogName(extractData(configData, "name"))),
    m_standardDebugStream(DebugMessage::getStream()){

    bool useExternalFile = (configData.Attribute("config") != NULL);

    // Obtain the configuration file if present, otherwise expect that the configuration is provided in-line
    const TiXmlElement* configSrcRoot = (useExternalFile ? initXml(findFile(extractData(configData, "config").toString()).c_str()) : &configData);

    // Should always be true
    Entity::gcRequired() = true;

    // Reset termination flag
    s_terminated = false;

    // Post static id.
    s_id = m_id;

    // This map will be populated as we read in the timeline modes for each reactor
    std::map<double, ServerId> serversByTimeline;

    std::vector<TiXmlElement*> elements;
    std::vector<TiXmlElement*> includedFiles; //Garbage to be deleted at the end.

    // Iterate over all TeleoReactors and allocate them
    for (TiXmlElement * child = configSrcRoot->FirstChildElement();
	 child != NULL;
	 child = child->NextSiblingElement()){
      elements.push_back(child);
    }
    while (elements.size()) {
      TiXmlElement * child = elements.at(0);
      elements.erase(elements.begin());
      static const char* DEFAULT = "TeleoReactor";

      // Test for a TeleoReactor. Allocate if we find one
      if(strcmp(child->Value(), "Include") == 0){
	std::string file = findFile(child->Attribute("name"));
	printf("Hi, i'm including: %s\n", file.c_str());
	TiXmlElement* iroot = EUROPA::initXml(file.c_str());
	includedFiles.push_back(iroot);
	for (TiXmlElement * ichild = iroot->FirstChildElement();
	     ichild != NULL;
	     ichild = ichild->NextSiblingElement()){
	  elements.push_back(ichild);
	}
      }else if(strcmp(child->Value(), "TeleoReactor") == 0){
	const char* component = child->Attribute("component");
	
	// If no component name is provided, use the default
	if(component == NULL)
	  component = DEFAULT;
	
	TeleoReactorId reactor = TeleoReactor::createInstance(m_name, component, *child);
	
	if(getReactor(reactor->getName()).isId()){
	  TREXLog() << reactor->getName().toString() << " is not unique. It must be.";
	  exit(-1);
	}
	
	m_reactorsByName.insert(std::pair<double, TeleoReactorId>(reactor->getName(), reactor));
	m_reactors.push_back(reactor);
	
	// Fill Data structures for observables etc so we can hookup correctly.
	std::list<LabelStr> externals, internals;
	reactor->queryTimelineModes(externals, internals);
	
	// Populate observers by timeline - external timelines are observers
	for(std::list<LabelStr>::const_iterator it = externals.begin(); it != externals.end(); ++it){
	  const LabelStr& timelineName = *it;
	  m_observersByTimeline.insert(std::pair<LabelStr, ObserverId>(timelineName, reactor->toObserver()));
	  debugMsg("trex:info:configuration", "Adding reactor " << reactor->getName().toString() << " as observer for " << timelineName.toString());
	}
	
	// For all the internal timelines, this reactor is a server, so add to our collection which will be used for binding
	for(std::list<LabelStr>::const_iterator it = internals.begin(); it != internals.end(); ++it){
	  const LabelStr& timelineName = *it;
	  
	  if(serversByTimeline.find(timelineName) != serversByTimeline.end()){
	    TREXLog() << "Configuration Error. Already have a server for " << timelineName.toString();
	    exit(-1);
	  }
	  
	  if( reactor->shouldLog() )
	    m_obsLog.declTimeline(timelineName, reactor->getName().toString());
	  
	  serversByTimeline.insert(std::pair<double, ServerId>(timelineName, reactor->toServer()));
	  debugMsg("trex:info:configuration", "Adding reactor " << reactor->getName().toString() << " as server for " << timelineName.toString());
	}
      }else{
	TREXLog() <<  "Invalid XML Tag - " << child->Value();
	exit(-1);
      }
    }

    while(!includedFiles.empty()){
      delete includedFiles[0];
      includedFiles.erase(includedFiles.begin());
    }
    

    // Reactors loaded : I can close the log header 
    m_obsLog.endHeader(getCurrentTick());

    // Now we should have built up the map for servers and so we can initialize the reactors with final communication binding
    for(std::vector<TeleoReactorId>::const_iterator it = m_reactors.begin(); it != m_reactors.end(); ++it){
      TeleoReactorId reactor = *it;
      reactor->doHandleInit(0, serversByTimeline, m_thisObserver);
    }

    m_sortedReactors = m_reactors;

    // Sort for suitable dependency graph, uses a priority comparator
    TeleoReactor::sort(m_sortedReactors);

    // Initialize deliberation agenda
    m_deliberators = m_sortedReactors;

    // Deallocate configuration root
    if(useExternalFile)
      delete configSrcRoot;
  }

  Agent::~Agent() {
    // Reset the Debug Message Stream before deallocating any reactors
    DebugMessage::setStream(m_standardDebugStream);

    s_terminated = true;

    // Delete all the reactors
    cleanup(m_reactorsByName);

    // Garbage collect any remaining entities
    Entity::garbageCollect();

    // Release the id, deallocating the memory
    m_thisObserver.release();

    // Cleanup all the remaining listeners. Use this while loop instead of an iterator
    // because listeners will call back to unregister when deleted, so the underlying buffer
    // here will change
    while(!m_listeners.empty()){
      AgentListenerId l = m_listeners.front();
      delete (AgentListener *) l;
    }

    m_id.remove();
  }

  /**
   * @brief Run the agent to completion.
   */
  void Agent::run(){
    m_clock.doStart();    
    LogManager::instance().handleInit();

    while(!terminated() && doNext()){}

    m_obsLog.endFile();
    debugMsg("Agent:run", "Completed at tick " << m_currentTick);

    if(!terminated())
      terminate();
  }

  void Agent::terminate(){
    debugMsg("Agent:terminate", "Terminating the Agent."); 
    s_terminated = true;
  }

  bool Agent::terminated(){
    return s_terminated;
  }

  /**
   * Each call to obtain the next clock tick may result in a transition to the next tick.
   */
  bool Agent::doNext(){

    if(missionCompleted())
      return false;

    handleTickStart();

    synchronize();

    // Deliberate as necessary while we have cpu available.
    while(m_clock.getNextTick() == m_currentTick && executeReactor()) {}

    // Wait for next tick
    while(m_clock.getNextTick() == m_currentTick){m_clock.sleep();}

    // Output results
    m_monitor.addTickData(m_synchUsage.user_time(), m_deliberationUsage.user_time());
    m_synchUsage.reset();
    m_deliberationUsage.reset();

    // Advance the tick
    m_currentTick++;


    return true;
  }

  bool Agent::executeReactor(){

    TeleoReactorId reactor = nextReactor();
    if(reactor.isId()){
      RStatLap chrono(m_deliberationUsage, RStat::self);
      reactor->doResume();
      return true;
    }
    return false;
  }

  /**
   * @brief Selection of first available reactor
   */
  TeleoReactorId Agent::nextReactor(){
    static unsigned int sl_counter(0);

    debugMsg("Agent:nextReactor", "[" << sl_counter << "] Size=" << m_deliberators.size());
    sl_counter++;

    if(m_deliberators.empty())
      return TeleoReactorId::noId();

    if(m_deliberators[0]->getLookAhead() > 0 && m_deliberators[0]->hasWork())
      return m_deliberators[0];

    // Remove the reactor that is done
    m_deliberators.erase(m_deliberators.begin());

    // Tail-recursion
    return nextReactor();
  }

  const AgentId& Agent::getId() const { return m_id; }

  const LabelStr& Agent::getName() const {return m_name;}

  TICK Agent::getCurrentTick() const {
    return m_currentTick;
  }

  TICK Agent::getFinalTick() const {
    return m_finalTick;
  }

  const Clock& Agent::getClock() const {
    return m_clock;
  }

  const TeleoReactorId& Agent::getReactor(const LabelStr& name){
    std::map<double, TeleoReactorId>::const_iterator it = m_reactorsByName.find(name);
    if(it == m_reactorsByName.end())
      return TeleoReactorId::noId();
    else
      return it->second;
  }

  const TeleoReactorId& Agent::getOwner(const LabelStr& timeline){
    if(m_ownersByTimeline.empty()){
      for(std::vector<TeleoReactorId>::const_iterator it = m_reactors.begin(); it != m_reactors.end(); ++it){
	TeleoReactorId reactor = *it;
	std::list<LabelStr> externals, internals;
	reactor->queryTimelineModes(externals, internals);
	for(std::list<LabelStr>::const_iterator l_it = internals.begin(); l_it != internals.end(); ++l_it){
	  const LabelStr& t = *l_it;
	  m_ownersByTimeline.insert(std::pair<double, TeleoReactorId>(t, reactor));
	}
      }
    }

    std::map<double, TeleoReactorId>::const_iterator it = m_ownersByTimeline.find(timeline);
    checkError(it != m_ownersByTimeline.end(), "No owner for " << timeline.toString());
    return it->second;
  }

  void Agent::logRequest(const TokenId& goal){
    debugMsg("Agent:logRequest", goal->toString());
    if(m_enableEventLogger){
      ObjectId object = (ObjectId) goal->getObject()->lastDomain().getSingletonValue();
      m_eventLog.push_back(Agent::Event(getCurrentTick(), Agent::Request, object->getName().toString(), goal->getPredicateName()));
    }
  }

  void Agent::logRecall(const TokenId& goal){
    debugMsg("Agent:logRecall", goal->toString());
    if(m_enableEventLogger){
      ObjectId object = (ObjectId) goal->getObject()->lastDomain().getSingletonValue();
      m_eventLog.push_back(Agent::Event(getCurrentTick(), Agent::Recall, object->getName().toString(), goal->getPredicateName()));
    }
  }

  /**
   * @brief Goes through the observersByTimeline structure set up on initialization and multi-casts to them
   */
  void Agent::notify(const Observation& observation){
    debugMsg("Agent:notify", observation.toString());
    TREXLog() << observation.toString() << std::endl;

    if(m_enableEventLogger)
      m_eventLog.push_back(Agent::Event(getCurrentTick(), Agent::Notify, observation.getObjectName(), observation.getPredicate()));

    m_obsLog.log(observation);

    for(std::multimap<LabelStr, ObserverId>::const_iterator it = m_observersByTimeline.find(observation.getObjectName());
	it != m_observersByTimeline.end(); ++it){
      // If we have moved past it, quit
      if(it->first != observation.getObjectName())
	return;

      // Otherwise, pass it on.
      ObserverId observer = it->second;
      observer->notify(observation);
    }
  }

  /**
   * Synchronization algorithm at the tick transition, The reactors are processed in a fixed sequence
   * based on the dependency graph which provides a static order and forms a DAG.
   */
  void Agent::synchronize() {
    RStatLap chrono(m_synchUsage, RStat::self);
    std::vector<TeleoReactorId>::const_iterator it = m_sortedReactors.begin();
    while(it != m_sortedReactors.end() && !terminated()){
      TeleoReactorId r = *it;
      if(!r->doSynchronize())
	throw new SynchronizationFailure();
      ++it;
    }

    LogManager::instance().handleNewTick(m_currentTick);
  }

  void Agent::handleTickStart(){

    debugMsg("Agent:handleTickStart", "Tick " << m_currentTick << " for " << getName().toString());

    // Reset
    m_synchUsage.reset();
    m_deliberationUsage.reset();

    // Reset the collection of deliberators
    m_deliberators = m_sortedReactors;

    // Iterate over all reactors and pass on the message
    std::vector<TeleoReactorId>::const_iterator it = m_reactors.begin();
    while(it != m_reactors.end() && !terminated()){
      TeleoReactorId reactor = *it;
      reactor->doHandleTickStart();
      ++it;
    }

  }

  bool Agent::missionCompleted() const{
    bool result = (m_currentTick >= m_finalTick);
    condDebugMsg(result, "Agent:missionCompleted", "Completed at tick " << m_currentTick);
    return result;
  }

  const std::vector<Agent::Event>& Agent::getEventLog() const { return m_eventLog;}


  void Agent::registerListener(const AgentListenerId& listener){
    m_listeners.push_back(listener);
  }

  void Agent::unregisterListener(const AgentListenerId& listener){
    for(std::list<AgentListenerId>::iterator it = m_listeners.begin(); it != m_listeners.end(); ++it){
      AgentListenerId l = *it;
      checkError(l.isValid(), l);
      if(l == listener){
	m_listeners.erase(it);
	return;
      }
    }
  }

  std::string Agent::toString(const  std::vector<Event>& eventLog){
    std::stringstream ss;
    for(unsigned int i = 0;i<eventLog.size(); i++){
      std::string evType;

      if(eventLog[i].m_eventType == Agent::Notify)
	evType = "NOTIFY ";
      else  if(eventLog[i].m_eventType == Agent::Request)
	evType = "REQUEST";
      else
	evType = "RECALL";

      ss << eventLog[i].m_tick << " " << evType << " " << eventLog[i].m_objectName.toString() << " " << eventLog[i].m_predicateName.toString() << std::endl;
    }

    return ss.str();
  }

  Agent::Event::Event(TICK tick, Agent::EventType evType, const LabelStr& objectName, const LabelStr& predicateName)
    : m_tick(tick), m_eventType(evType), m_objectName(objectName), m_predicateName(predicateName) {}

  Agent::Event::Event(const Agent::Event& org)
    : m_tick(org.m_tick), m_eventType(org.m_eventType), m_objectName(org.m_objectName), m_predicateName(org.m_predicateName) {}


  const LabelStr& Agent::TIMELINE(){
    static const LabelStr sl_action("AgentTimeline");
    return sl_action;
  }

  const LabelStr& Agent::ACTION(){
    static const LabelStr sl_action("AgentActions");
    return sl_action;
  }

  const LabelStr& Agent::TIMELINE_MODE(){
    static const LabelStr sl_lbl("mode");
    return sl_lbl;
  }

  const LabelStr& Agent::INTERNAL_TIMELINE(){
    static const LabelStr sl_lbl("Internal");
    return sl_lbl;;
  }

  const LabelStr& Agent::EXTERNAL_TIMELINE(){
    static const LabelStr sl_lbl("External");
    return sl_lbl;
  }

  const LabelStr& Agent::IGNORE_TIMELINE(){
    static const LabelStr sl_lbl("Ignore");
    return sl_lbl;
  }

  LabelStr Agent::buildLogName(LabelStr const &prefix) {
    std::ostringstream oss;
    
    oss<<prefix.toString()<<".log";
    return oss.str();
  }

  void Agent::setMonitor(PerformanceMonitor& monitor){
    m_monitor = monitor;
  }

  const PerformanceMonitor& Agent::getMonitor()const {
    return m_monitor;
  }

  void Agent::notifyRejected(const TokenId token){
    for(std::list<AgentListenerId>::const_iterator it = m_listeners.begin(); it != m_listeners.end(); ++it){
      AgentListenerId l = *it;
      checkError(l.isValid(), l);
      l->notifyRejected(token);
    }
  }

  void Agent::notifyCompleted(const TokenId& token){
    for(std::list<AgentListenerId>::const_iterator it = m_listeners.begin(); it != m_listeners.end(); ++it){
      AgentListenerId l = *it;
      checkError(l.isValid(), l);
      l->notifyCompleted(token);
    }
  }

  std::ostream& Agent::getStream(){return m_standardDebugStream;}

}
