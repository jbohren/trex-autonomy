/* -*- C++ -*-
 * $Id$
 */
/** @file "SimAdapter.hh"
 * @brief Declaration of SimAdapter
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#ifndef _SIMADAPTER_HH
# define _SIMADAPTER_HH

#include <set>

#include "TeleoReactor.hh"
#include "IntervalTypeFactory.hh"
#include "IntervalIntTypeFactory.hh"
#include "BoolTypeFactory.hh"
#include "StringTypeFactory.hh"
#include "SymbolTypeFactory.hh"

namespace TREX {
  
  /** @brief A log play %TeleoReactor
   *
   * This class is able to play any Timeline declared in a log file produced by ObservationLogger.
   *
   * @author Frederic Py <fpy@mbari.org>
   */
  class SimAdapter :public TeleoReactor {
  public:
    enum DomainType { ANY = 0, BOOL, INT, FLOAT, STRING, SYMBOL, OBJECT, REAL_INTERVAL, INT_INTERVAL } ;

    /** @brief Constructor
     *
     * @param agentName name of the agent
     * @param configData Configuration data for this instance
     */
    SimAdapter(LabelStr const &agentName, TiXmlElement const &configData);
    /** @brief Destructor
     */
    ~SimAdapter();

    /** @brief Handle intialization signel sent by the Agent
     */
    void handleInit(TICK initialTick, 
		    std::map<double, ServerId> const &serversByTimeline,
		    ObserverId const &observer);

    /** @brief Synchronization
     *
     * This method will play all the Observation for current tick
     * as defined in the log file.
     */
    bool synchronize();

    void notify(Observation const &observation);
    bool handleRequest(TokenId const &token);
    void handleRecall(TokenId const &token);

    void queryTimelineModes(std::list<LabelStr> &externals, 
			    std::list<LabelStr> &internals);
    
    void terminate() {}

    bool isExhausted() const {return m_lastBacktracked == (int) getCurrentTick();}
    void backtrack(){m_lastBacktracked = getCurrentTick();}

  private:
    ObserverId m_observer; //!< Observer connect to the Agent
    std::set<LabelStr> m_internals; /*!< The timelines it will accept goals on and issue observations */
    std::multimap<TICK, Observation *> m_log; //!< Observations extracted from log file
    std::multimap<TICK, Observation *>::iterator m_nextObs; //!< next observation to play
    int m_lastBacktracked;
    IntervalTypeFactory m_floatTypeFactory;
    IntervalIntTypeFactory m_intTypeFactory;
    BoolTypeFactory m_boolTypeFactory;
    StringTypeFactory m_stringTypeFactory;
    SymbolTypeFactory m_symbolTypeFactory;

    bool hasWork() { return false; }
    void resume() {}
    void archive() {}

    /** @brief Observations loading
     *
     * @param file A log file content
     *
     * This file extract all the Observation of @e file corresponding to the timeline
     * in m_internals into m_log to prepare the reactor to play them.
     */
    void loadObservations(TiXmlDocument const &file);

    /** @brief Parsing EnumeratedDomain from XML
     *
     * @param elem A XML element
     *
     * @return The result of parsing
     */
    EnumeratedDomain *xmlAsEnumeratedDomain(TiXmlElement const &elem);

    /** @brief Parsing IntervalDomain from XML
     *
     * @param elem A XML element
     *
     * @return The result of parsing
     */
    IntervalDomain *xmlAsIntervalDomain(TiXmlElement const &elem);

    /** @brief Parsing AbstractDomain from XML
     *
     * @param elem A XML element
     *
     * @return The result of parsing
     */
    AbstractDomain *xmlAsAbstractDomain(TiXmlElement const &elem);

    /** @brief Parsing Observation from XML
     *
     * @param elem A XML element
     *
     * @return The result of parsing
     */
    Observation *xmlAsObservation(TiXmlElement const &elem);

    /**
     * @brief Utilities to get a factory by type
     */
    TypeFactory& getFactory(DomainType t);
    TypeFactory& getFactory(const std::string& t);
  }; // TREX::SimAdapter

} // TREX

#endif // _SIMADAPTER_HH
