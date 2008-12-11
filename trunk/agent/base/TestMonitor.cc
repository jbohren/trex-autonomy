#include "TestMonitor.hh"

namespace TREX {

  bool TestMonitor::success(){
    // Iterate over all entries. Each one must be resolved and the expected and actual values must match
    for(std::list<TestMonitor::Entry>::const_iterator it = entries().begin(); it != entries().end(); ++it){
      const Entry& entry = *it;
      if(!entry.resolved || entry.expectedValue != entry.actualValue)
	return false;
    }

    return true;
  }

  void TestMonitor::reset(){
    entries().clear();
  }

  std::string TestMonitor::toString(){
    std::stringstream ss;
    for(std::list<TestMonitor::Entry>::const_iterator it = entries().begin(); it != entries().end(); ++it){
      const Entry& entry = *it;
      ss << entry.label << "(";
      ss << (entry.resolved ? "resolved" : "unresolved") << ", ";
      ss << (entry.expectedValue == entry.actualValue ? "success" : "failure") << ")\n";
    }

    return ss.str();
  }
}
