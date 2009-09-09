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

#include <stdio.h>
#include <string>
#include "Utilities.hh"
#include "XMLUtils.hh"
#include <fstream>
#include "ModulePlanDatabase.hh"
#include "ModuleRulesEngine.hh"
#include "ModuleNddl.hh"
#include "ModuleConstraintEngine.hh"


using namespace EUROPA;
using namespace TREX;


class NddlReader: public EngineBase {
  public:
  NddlReader() {
    addModule((new ModuleConstraintEngine())->getId()); 
    addModule((new ModulePlanDatabase())->getId());
    addModule((new ModuleRulesEngine())->getId());
    addModule((new ModuleNddl())->getId());
    doStart();
  }

  void read(std::string file) {
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
	  getLanguageInterpreter("nddl")->getEngine()->getConfig()->setProperty("nddl.eval", "false");
	}
      }
    }
    try {
      std::string ret = executeScript("nddl", file, true);
      assertTrue(ret == "", "Parser failed with return: " + ret);
    } catch(std::string ex) {
      assertTrue(false, "Parser failed: " + ex);
    } catch(...) {
      assertTrue(false, "Parser failed with unknown exception.");
    }
  }
  
};

int main(unsigned int argc, char **argv) {
  printf("This is the NDDL Reader.\n");
  if (argc != 2) {
    printf("Usage: %s <nddl-file>\nThe file <nddl-file> will be read and a log of parsing displayed. If there is an error, parsing will halt and it will be reported.", argv[0]);
    return 1;
  }


  DebugMessage::enableMatchingMsgs("", "NddlInterpreter");
  NddlReader *r = new NddlReader();
  r->read(argv[1]);
  delete r;

  return 0;
}
