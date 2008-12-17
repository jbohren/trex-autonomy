#ifndef H_AgentListener
#define H_AgentListener

/**
 * @author Conor McGann
 * @file Provides an abstract base class listener interface (AgentListener) for plug-ins to capture events on goal execution
 */

#include "Agent.hh"

namespace TREX {

  class AgentListener {
  public:

    /**
     * @brief Constructor will register with the agent singleton
     */
    AgentListener(): m_id(this) {Agent::instance()->registerListener(m_id);}

    /**
     * @brief Destructor will unregister
     */
    virtual ~AgentListener(){
      Agent::instance()->unregisterListener(m_id);
      m_id.remove();
    }

    const AgentListenerId& getId(){ return m_id; }

    // Subclass should implement these methods
    virtual void notifyRejected(const TokenId& token) = 0;
    virtual void notifyCompleted(const TokenId& token) = 0;

  private:
    AgentListenerId m_id;
  };

}
#endif
