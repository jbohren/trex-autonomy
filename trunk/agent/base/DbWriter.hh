/**
 * @brief Derived from EUROPA SolverPartialPlanWriter
 * @author Conor McGann, Willow Garge, Inc.
 */

#ifndef H_DbWriter
#define H_DbWriter

#include "LabelStr.hh"
#include "PlanDatabaseDefs.hh"
#include "ConstraintEngineDefs.hh"
#include "RulesEngineDefs.hh"
#include "XMLUtils.hh"

#include "Agent.hh"

#include <set>
#include <map>
#include <vector>

#ifdef ERROR
#undef ERROR
#endif

namespace TREX {

  using namespace EUROPA;

  class DbWriter {
  public:
    DbWriter(const std::string& agentName, const std::string& reactorName, 
	     const PlanDatabaseId& dbId, const ConstraintEngineId& ceId,
	     const RulesEngineId& reId2);

    virtual ~DbWriter();
    virtual void write(TICK tick);
    virtual void writeStatistics();

    void addSourcePath(const char* path);

  protected:
    inline long long int getPPId(void){return ppId;}
    long long int ppId;
    long long int seqId;

  private:

    std::string m_agentName;
    std::string m_reactorName;
    std::string m_targetDir;
    std::string dest;
    PlanDatabaseId pdbId;
    ConstraintEngineId ceId;
    RulesEngineId reId;
    int stepCount;
    bool destAlreadyInitialized;
    double m_writing;
    int numTokens, numConstraints, numVariables;

    std::ofstream *statsOut, *ruleInstanceOut;
    std::list<std::string> sourcePaths;

    void initOutputDestination();
    void outputObject(const ObjectId &, const int, std::ofstream &, std::ofstream &);
    void outputToken(const TokenId &, const int, const int, const int, const int, 
		     const ObjectId &, std::ofstream &, std::ofstream &);
    void outputStateVar(const Id<TokenVariable<StateDomain> >&, const int, const int,
			std::ofstream &varOut);
    void outputEnumVar(const Id< TokenVariable<EnumeratedDomain> > &, const int,
		       const int, std::ofstream &);
    void outputIntVar(const Id< TokenVariable<IntervalDomain> > &, const int,
		      const int, std::ofstream &);
    void outputIntIntVar(const Id< TokenVariable<IntervalIntDomain> >&, const int,
			 const int, std::ofstream &);
    void outputObjVar(const ObjectVarId &, const int, const int,
		      std::ofstream &);
    void outputConstrVar(const ConstrainedVariableId &, const int, const int, 
			 std::ofstream &);
    void outputConstraint(const ConstraintId &, std::ofstream &, std::ofstream &);
#ifndef NO_RESOURCES
    // TODO JRB: Move this to Resource module 
    //void outputInstant(const InstantId &, const int, std::ofstream &);
#endif
    void outputRuleInstance(const RuleInstanceId &, std::ofstream &, std::ofstream & , std::ofstream &);
    void buildSlaveAndVarSets(std::set<TokenId> &, std::set<ConstrainedVariableId> &, 
			      const RuleInstanceId &);
    void writeStats(void);
    void collectStats(void);
    const std::string getUpperBoundStr(IntervalDomain &dom) const;
    const std::string getLowerBoundStr(IntervalDomain &dom) const;
    const std::string getEnumerationStr(EnumeratedDomain &dom) const;
    const std::string getVarInfo(const ConstrainedVariableId &) const;
    const bool isCompatGuard(const ConstrainedVariableId &) const;
  };
}

#endif
