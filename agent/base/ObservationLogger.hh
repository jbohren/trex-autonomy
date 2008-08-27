/* -*- C++ -*-
 * $Id$
 */
/** @file "ObservationLogger.hh"
 * @brief Definition of ObservationLogger class
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#ifndef _OBSERVATIONLOGGER_HH
# define _OBSERVATIONLOGGER_HH

#include <cstdio>
#include "TREXDefs.hh"
#include "Observer.hh"

namespace TREX {  
  
  /** @brief Observation Logger for Reactors.
   *
   * This class is used by Agent to log observations sent
   * by an agent. The produced log file can then be used by
   * SimAdapter class to replay the mission.
   *
   * @author Frederic Py <fpy@mbari.org>
   */
  class ObservationLogger {
  public:
    /** @brief Add a Timeline to log.
     *
     * @param name The name of the Timeline
     * @param owner The name of the Timeline owner
     *
     * This method indicates to current instance to log
     * Observation on Timeline @e name.
     *
     * @pre This method can be called iff endHeader
     * was not called yet. Indeed t adds information on the
     * Header of the log file.
     *
     * @post The Header of the log file will declare Timeline @e name
     * has owned by @e owner
     * @post The Observation notified by @e owner on @e name will be logged. 
     */
    void declTimeline(LabelStr const &name, LabelStr const &owner);

    /** @brief close the log header
     *
     * @param init Value of initial tick
     *
     * This method close the header section of the log file.
     * If the header is empty (i.e. no previous call to declTimeline) then
     * no log file is produced. Otherwise this instance starts to log
     * Observation on timelined declared as to be logged.
     *
     * @pre endHeader was never called. 
     *
     * @sa declTimeline(LabelStr const &name, LabelStr const &owner) 
     */
    void endHeader(TICK init);
    /** @brief Log a new observation
     *
     * @param obs an Observation
     *
     * This method is used by Agent to indicate to current instance
     * that Observation @e obs occured. If @e obs is attached to a
     * Timeline declared as to be logged. This one will be appended
     * in the log file     
     *
     * @pre endHeader was previously called.
     */
    void log(Observation const &obs);

  private:  
    bool m_inHeader; //!< Flag to indicate if we are still in header
    bool m_empty; //!< Flag to indicate that no Observation are logged yet 
    LabelStr m_logName; //!< Name of the log file
    FILE *m_logFile; //!< Log file

    std::map<LabelStr, LabelStr> m_timelines; //!< List of timelines to log
    TICK m_lastTick; //!< Value of current tick 
    bool m_hasData; //!< Flag to indicate if any Observation was already logged during this tick

    /** @brief Constructor.
     *
     * @param fileName The base name for the log file
     */
    ObservationLogger(LabelStr const &fileName);
    /** @brief Destructor
     */
    ~ObservationLogger();

    /** @brief Open and start the log file
     *
     * This method create the log file and open the header for
     * Timeline declaration.
     */
    void startFile();
    /** @brief Close the log file.
     */
    void endFile();

    friend class Agent;
  }; // TREX::ObservationLogger

} // TREX

#endif // _OBSERVATIONLOGGER_HH
