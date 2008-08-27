/**
 * @author Conor McGann
 * @brief Defines a performance monitor interface for use by the agent
 */
#include <vector>

namespace TREX {

  class PerformanceMonitor {
  public:
    virtual ~PerformanceMonitor(){}

    virtual void addTickData(const timeval& synchTime, const timeval& deliberationTime){
      m_tickData.push_back(std::pair<timeval, timeval>(synchTime, deliberationTime));
    }

    static PerformanceMonitor& defaultMonitor(){
      static PerformanceMonitor sl_instance;
      return sl_instance;
    }

    const std::vector< std::pair<timeval, timeval> >& getData() const {return m_tickData;}

  protected:
    std::vector< std::pair<timeval, timeval> > m_tickData;
  };


  class StatisticsCollector: public PerformanceMonitor {
  public:
    void addTickData(const timeval& synchTime, const timeval& deliberationTime){
      m_tickData.push_back(std::pair<timeval, timeval>(synchTime, deliberationTime));
    }
  };
}
