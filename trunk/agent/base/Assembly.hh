#ifndef _H_Assembly
#define _H_Assembly

/**
 * @file Assembly.hh
 * @note Adapted from Assembly structure of EUROPA
 */

#include "PlanDatabaseDefs.hh"
#include "RulesEngineDefs.hh"
#include "Module.hh"
#include "Engine.hh"
#include "DbWriter.hh"

using namespace EUROPA;

namespace TREX {

  /**
   * @brief Provides a simple facade over standard components.
   *
   * The standard assembly includes all major EUROPA components for the PlanDatabase:
   * @li ConstraintEngine
   * @li PlanDatabase
   * @li RulesEngine
   * @li Resources
   * @li TemporalNetwork
   */
  class Assembly: public EngineBase {
  public:

    /**
     * @brief Constructor
     * @param agentName The agent name, used to direct DbWriter output
     * @param reactorName The reactor name, also used to direct DbWriter output
     */
    Assembly(const LabelStr& agentName, const LabelStr& reactorName);

    /**
     * @brief Deallocate all data associated with this instance.
     */
    virtual ~Assembly();

    /**
     * @brief Play Transactions stored in the given file
     */
    bool playTransactions(const char* txSource);

    const SchemaId& getSchema() const {return m_schema;}
        
    const ConstraintEngineId& getConstraintEngine() const { return m_constraintEngine; }

    const PlanDatabaseId& getPlanDatabase() const { return m_planDatabase; }

    const RulesEngineId&  getRulesEngine() const { return m_rulesEngine; }

    /**
     * @brief Export Plan Database for Inspection by PlanWorks
     */
    const std::string& exportToPlanWorks(TICK tick, unsigned int attempt);

    /**
     * @brief Inner class to allow schema registration functions to be invoked. This is necessary because
     * component factory registration is schema specific
     */
    class Schema {
    public:
      /**
       * @brief Singleton accessor. Will allocate an instance of none present
       */
      static Schema* instance();

      /**
       * @brief A Default constructor
       */
      Schema();

      /**
       * @brief Destructor will reset the instance pointer
       */
      virtual ~Schema();

      /**
       * @brief Call back on the schema. Allows customization of schemas for an assembly.
       * @param assembly instance from which schema can be obtained.
       */
      virtual void registerComponents(const Assembly& assembly);

    private:
      static Schema* s_instance; /*! Singleton pointer accessor */
    };

  protected:

    Assembly();

    /**
     * Accessor for partial plan writer. Create on demand.
     */
    DbWriter* getPPW();

    const LabelStr m_agentName;
    const LabelStr m_reactorName;
    SchemaId m_schema;
    ConstraintEngineId m_constraintEngine;
    PlanDatabaseId m_planDatabase;
    RulesEngineId m_rulesEngine;    

    DbWriter* m_ppw; // Optional. Load on demand.
  };
}
#endif
