/*********************************************************************
* Software License Agreement (BSD License)
* 
*  Copyright (c) 2007. MBARI.
*  All rights reserved.
* 
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
* 
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the TREX Project nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*/

#include "NddlReader.hh"

NddlReader::NddlReader() {
  addModule((new ModuleConstraintEngine())->getId());
  addModule((new ModuleConstraintLibrary())->getId());
  addModule((new ModulePlanDatabase())->getId());
  addModule((new ModuleRulesEngine())->getId());
  addModule((new ModuleTemporalNetwork())->getId());
  addModule((new ModuleNddl())->getId());
  doStart();
}

void NddlReader::read(std::string file) {
  ConstraintEngine* constraintEngine = ((ConstraintEngine*)getComponent("ConstraintEngine"))->getId();
  
  
  new DefaultPropagator(LabelStr("OnCommit"), constraintEngine->getId());
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), SetDefaultOnCommit, "defaultOnCommit", "OnCommit");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), AbsMaxOnCommit, "absMaxOnCommit", "OnCommit");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), SetDefault, "default", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), SetDefault, "bind", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), LessThanConstraint, "lt", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TestLessThan, "testLT", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), Neighborhood, "neighborhood", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::CompletionMonitorConstraint, "assertCompleted", "Default"); //Need dummy constraints here.
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::RejectionMonitorConstraint, "assertRejected", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsStarted, "isStarted", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsEnded, "isEnded", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsTimedOut, "isTimedOut", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsSucceded, "isSucceded", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsAborted, "isAborted", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::IsPreempted, "isPreempted", "Default");
  REGISTER_CONSTRAINT(constraintEngine->getCESchema(), TREX::MasterSlaveRelation, "trex_behavior", "Default");
  
  registerConstraints();


  std::ifstream f1(findFile("NDDL.cfg").c_str());
  std::ifstream f2(findFile("temp_nddl_gen.cfg").c_str());
  TiXmlElement* iroot = NULL;
  if (f1.good()) {
    iroot = EUROPA::initXml(findFile("NDDL.cfg").c_str());
  } else if (f2.good()) {
    iroot = EUROPA::initXml(findFile("temp_nddl_gen.cfg").c_str());
  } else {
    checkError(false, "Could not find 'NDDL.cfg' or 'temp_nddl_gen.cfg'");
  }
  if (iroot) {
    for (TiXmlElement * ichild = iroot->FirstChildElement();
	 ichild != NULL;
	 ichild = ichild->NextSiblingElement()) {
      if (std::string(ichild->Value()) == "include") {
	std::string path = std::string(ichild->Attribute("path"));
	for (unsigned int i = 0; i < path.size(); i++) {
	  if (path[i] == ';') {
	    path[i] = ':';
	  }
	}
	getLanguageInterpreter("nddl")->getEngine()->getConfig()->setProperty("nddl.includePath", path);
      }
      if (std::string(ichild->Value()) == "trex-path") {
	std::string path = std::string(ichild->Attribute("path"));
	setenv("TREX_PATH", path.c_str(), 1);
      }
    }
  }


  setenv("TREX_NO_EVAL", "1", 1);


  setenv("TREX_START_DIR", file.substr(0, file.rfind("/")).c_str(), 1);
  printf("%s\n", getenv("TREX_START_DIR"));


  try {
    std::string ret = executeScript("nddl", file, true);
    assertTrue(ret == "", "Parser failed with return: " + ret);
  } catch(std::string ex) {
    assertTrue(false, "Parser failed: " + ex);
  } catch(...) {
    assertTrue(false, "Parser failed with unknown exception.");
  }
}

void NddlReader::registerConstraints() {
}

int NddlReaderMain(unsigned int argc, char **argv) {
  printf("This is the NDDL Reader.\n");
  if (argc != 2) {
    printf("Usage: %s <nddl-file>\nThe file <nddl-file> will be read and a log of parsing displayed. If there is an error, parsing will halt and it will be reported.\n", argv[0]);
    return 1;
  }
  DebugMessage::enableMatchingMsgs("", "NddlInterpreter");
  return 0;
}



