#ifndef H_GreedyOpenConditionManager
#define H_GreedyOpenConditionManager

#include "OpenConditionManager.hh"
#include "FlawFilter.hh"
#include "Token.hh"
#include "TokenVariable.hh"

/**
 * @brief The goal manager.
 */

using namespace EUROPA::SOLVERS;

namespace TREX {

  /**
   * @brief Same as parent class but we over-ride the comparator to use the earlier of the 2
   */
  class GreedyOpenConditionManager: public OpenConditionManager {
  public:

    /**
     * @brief Uses standard constructor
     */
    GreedyOpenConditionManager(const TiXmlElement& configData) : OpenConditionManager(configData){}

  protected:
    /**
     * @brief Comparator first prefers according to start time, and then by key
     */
    bool betterThan(const EntityId& a, const EntityId& b, LabelStr& explanation){
      if(b.isNoId()){
        explanation = "b.isNoId";
	return true;
      }

      TokenId tokenA(a);
      TokenId tokenB(b);

      explanation = "earlier";
      return tokenA->start()->lastDomain().getLowerBound() < tokenB->start()->lastDomain().getLowerBound();
    }
  };
}

#endif
