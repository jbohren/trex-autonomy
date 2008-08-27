#ifndef H_Observer
#define H_Observer

/**
 * @author Conor McGann
 * @file Declares observation related interfaces
 */
#include "TREXDefs.hh"
#include "LabelStr.hh"
#include "AbstractDomain.hh"
#include "PlanDatabaseDefs.hh"

# include "EuropaXML.hh"

namespace TREX {

  /**
   * @brief An observation is a predicate observed at current time.
   */
  class Observation {
  public:
    /**
     * @brief Return the attribute to which the observation applies
     */
    const LabelStr& getObjectName() const;

    /**
     * @brief Return the predicate of the observation
     */
    const LabelStr& getPredicate() const;

    /**
     * @brief Return the number of arguments contained
     */
    unsigned int countParameters() const;

    /**
     * @brief Accessor for the parameter values by position. Pure virtual to allow efficient underlying implementation.
     */
    virtual const std::pair<LabelStr, const AbstractDomain*> operator[](unsigned int index) const = 0;

    /**
     * @brief Utility to help tracing
     */
    std::string toString() const;
  
    /** @brief Utility to serialize data in XML format
     *
     * @sa ObservationLogger 
     */
    TiXmlElement *toXML() const;

    void printXML(FILE *out) const;

    /**
     * @brief Utility for getting the tokens timeline
     */
    static LabelStr getTimelineName(const TokenId& token);

    virtual ~Observation();

  protected:
    Observation(const LabelStr& objectName, const LabelStr& predicateName, unsigned int parameterCount = 0);
    unsigned int m_parameterCount;

  private:
    const LabelStr m_objectName;
    const LabelStr m_predicateName;
  };

  class ObservationByReference : public Observation {
  public:
    ObservationByReference(const TokenId& token);

    const std::pair<LabelStr, const AbstractDomain*> operator[](unsigned int) const;
  private:
    const TokenId m_token;
  };

  class ObservationByValue: public Observation {
  public:
    ObservationByValue(const LabelStr& objectName, const LabelStr& predicateName);

    ~ObservationByValue();

    const std::pair<LabelStr, const AbstractDomain*> operator[](unsigned int) const;

    void push_back(const LabelStr&, AbstractDomain* dom);

  private:
    std::vector< std::pair<LabelStr, AbstractDomain*> > m_parameters;
  };

  class Observer {
  public:
    virtual void notify(const Observation& observation) = 0;

    virtual ~Observer(){}
  };
}

#endif
