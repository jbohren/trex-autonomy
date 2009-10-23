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

#include <fstream>
#include "Utilities.hh"
#include "XMLUtils.hh"

int main(int argc, char **argv) {

  if (argc <= 1 || argc >= 4) {
    printf("Usage %s file [path]\n\tWhere file is the file to find, and path is the start_dir.\n", argv[0]);
    return 2;
  }

  char mypath[1024], wd[1024];

  if (argc == 3) {
    getcwd(wd, 1024);
    chdir(argv[2]);
  }

  setenv("TREX_START_DIR", getcwd(mypath, 1024), 1);
  if (argc == 3) {
    chdir(wd);
  }


  //printf("Start dir: %s\n", getenv("TREX_START_DIR"));

  //Load config.
  std::ifstream f1(TREX::findFile("NDDL.cfg").c_str());
  std::ifstream f2(TREX::findFile("temp_nddl_gen.cfg").c_str());
  TiXmlElement* iroot = NULL;
  if (f1.good()) {
    iroot = EUROPA::initXml(TREX::findFile("NDDL.cfg").c_str());
  } else if (f2.good()) {
    iroot = EUROPA::initXml(TREX::findFile("temp_nddl_gen.cfg").c_str());
  } else {
    printf("Could not find 'NDDL.cfg' or 'temp_nddl_gen.cfg', thus, not searching a path.\n");
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
	//printf("Path: %s\n", path.c_str());
	setenv("TREX_PATH", path.c_str(), 1);
      }
    }
  }

  std::string f = TREX::findFile(argv[1], true);
  std::ifstream fi(f.c_str());
  if (fi.good()) {
    printf("%s\n", f.c_str());
  } else {
    printf("File '%s' not found.\nPath: '%s'.\nStart Dir: '%s'.\n", argv[1], getenv("TREX_PATH"), getenv("TREX_START_DIR"));
    return 1;
  }
  return 0;
}



