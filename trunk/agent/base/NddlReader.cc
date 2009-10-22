
#include "Assembly.hh"
#include <stdio.h>
#include "Utilities.hh"
 
 
using namespace EUROPA;
using namespace TREX;

namespace trex_project { 
  void register_projects();
}

int main(int argc, char **argv) {
  // Call registration hook
  trex_project::register_projects();

  printf("This is the NDDL Reader.\n");
  if (argc != 2) {
    printf("Usage: %s <nddl-file>\nThe file <nddl-file> will be read and a log of parsing displayed. If there is an error, parsing will halt and it will be reported.\n", argv[0]);
    return 1;
  }

  DebugMessage::enableMatchingMsgs("", "NddlInterpreter");

  // Load the assembly and play the file
  TREX::Assembly assembly("NO_AGENT", "NO_REACTOR");
  assembly.playTransactions(argv[1]);
  return 0;
}
