#ifndef H_Server
#define H_Server

namespace TREX {

  /**
   * @brief Abstract interface for handling goals. 
   */
  class Server {
  public:

    /**
     * @brief Commands the server to handle a goal. This method should apply all the constraints attached to the
     * given token that it can reasonably support.
     * @param goal A token from the client database which is to be accomplished.
     * @return true if the request can be received, false if it cannot be received and should be resent
     */
    virtual bool request(const TokenId& goal) = 0;

    /**
     * @brief Commands the server to discard a goal.
     * @param goal A token from the client database which is to be recalled.
     */
    virtual void recall(const TokenId& goal) = 0;


    /**
     * @brief Retrieve the latency it takes to respond. This is used in planning when to dispatch goals. It is a lower bound on dispatch window.
     */
    virtual TICK getLatency() const = 0;

    /**
     * @brief Retrieve the look-ahead window. This is used in deciding how far ahead to commit goals. It is an upper bound on the dispatch window.
     */
    virtual TICK getLookAhead() const = 0;

    virtual ~Server(){}
  };
}

#endif
