/* -*- C++ -*-
 * $Id$
 */
/** @file "ObserverReactor.hh"
 * @brief Provide declarations for the ObserverReactor
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#ifndef H_ObserverReactor
# define H_ObserverReactor

# include <set>

# include "TeleoReactor.hh"

namespace TREX {
  
  class ObserverReactor: public TeleoReactor {
  public:
    ObserverReactor(LabelStr const &agentName, TiXmlElement const &configData);
    
    ~ObserverReactor();

    void queryTimelineModes(std::list<LabelStr> &externals, 
			    std::list<LabelStr> &internals);

    void handleInit(TICK initialTick, std::map<double, ServerId> const &serversByTimeline,
		    ObserverId const &observer);

    void notify(Observation const &obs);
    bool handleRequest(const TokenId& goal);
    void handleRecall(const TokenId& goal);

  protected:
    bool hasWork() { return false; }
    void resume() {}
    void archive() {}

    void handleTickStart() {}
    void handleTickEnd() {}

    static void getTimelines(std::map<LabelStr, ServerId> &resuls, 
			     TiXmlElement const &configData);
    static TiXmlElement const &externalConfig(TiXmlElement const &sourceConfig);

  private:
    std::map<LabelStr, ServerId> m_externals;

    std::string nameString() const; 

  }; // TREX::ObserverReactor

} // TREX

#endif // H_ObserverReactor
