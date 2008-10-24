/* -*- C++ -*-
 * $Id$
 */
/** @file "LogManager.cc"
 *
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cerrno>

#include "Debug.hh"

#include "TREXDefs.hh"
#include "LogManager.hh"

#include "MutexWrapper.hh"
#include "Guardian.hh"

using namespace TREX;
/*
 * class LogManager
 */ 

// statics :

std::auto_ptr<LogManager> LogManager::s_instance(0);

std::string LogManager::short_name(std::string const &file_name) {
  size_t slash = file_name.find_last_of('/');

  if( slash!=std::string::npos ) 
    return std::string(file_name, slash+1);
  return file_name;
}

LogManager &LogManager::instance() {
  static Mutex sl_mutex;

  if( 0==s_instance.get() ) {
    Guardian<Mutex> guard(sl_mutex);
    if( 0==s_instance.get() ) // double check in case another process did create it
      s_instance.reset(new LogManager);
  }
  return *s_instance;
}

// structors :

LogManager::LogManager() {
  char *base_dir = getenv("TREX_LOG_DIR");
  char dated_dir[17];

  dated_dir[16] = '\0';

  time_t cur_time;
  
  time(&cur_time);
  strftime(dated_dir, 16, "/%Y.%j.", gmtime(&cur_time));
  
  // First I check if latest exists
  std::string latest = (base_dir != NULL ? base_dir : ".");

  // Create if necessary
  debugMsg("LogManager", "Setting up log directory in:" << latest);
  static const mode_t  LOG_MODE = 0777; //S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH;
  mkdir(latest.c_str(), LOG_MODE);

  std::string cur_basis = latest;
  size_t len = latest.length()+30;
  char *buf = new char[len+1];

  memset(buf, 0, len+1);
  latest = latest+"/"+LATEST_DIR;
  cur_basis += dated_dir;
  
  int ret = readlink(latest.c_str(), buf, len-1);
  size_t last = 0;

  if( ret>0 ) {
    // The "latest" exist and point to something lets check it
    size_t b_len = cur_basis.length();
    if( cur_basis.compare(0, b_len, buf, b_len)==0 ) {
      std::istringstream iss(std::string(buf, b_len));
      
      iss>>last;
    }

    debugMsg("LogManager", "Unlinking latest directory");
    unlink(latest.c_str());
  }

  delete[] buf;

  for(unsigned i=1; i<MAX_LOG_ATTEMPT; ++i) {
    std::ostringstream oss;

    oss<<cur_basis<<(last+i);
    if(mkdir(oss.str().c_str(), LOG_MODE) != 0)
      continue;

    m_path = oss.str();
    // Create the "latest" symbolic link. As it is not  "critical" I am not checking it. 
    // Normally the unlink as already been done 
    //       unlink(latest.c_str());
    symlink(m_path.c_str(), latest.c_str());

    m_syslog.open(file_name(TREX_LOG_FILE).c_str());
    m_debug.open(file_name(TREX_DBG_FILE).c_str());

    DebugMessage::setStream(m_debug);

    debugMsg("LogManager", " logging directory is \""<<m_path<<'\"');
    return;
  }

  checkError(ALWAYS_FAIL, "LogManager: too many log dirs (MAX_LOG_ATTEMPT).");
}

LogManager::~LogManager() {
  std::map<std::string, TickLogger *>::iterator i = m_logs.begin(), 
    endi = m_logs.end();
  for( ;endi!=i ; ++i )
    delete i->second;
}

// Manipulators:

TickLogger *LogManager::getTickLog(std::string const &baseName) {
  std::pair<std::string, TickLogger *> to_ins(baseName, NULL);
  std::pair<std::map<std::string, TickLogger *>::iterator, bool> ret = m_logs.insert(to_ins);
  
  if( ret.second ) {
    debugMsg("LogManager", " creating numeric data log \""<<baseName<<'\"');
    ret.first->second = new TickLogger(baseName);
  }
  return ret.first->second;
}

// Observers :

void LogManager::handleInit() const {
  std::map<std::string, TickLogger *>::const_iterator i=m_logs.begin(), 
    endi = m_logs.end();
  for( ; endi!=i; ++i)
    i->second->printHeader();
}

void LogManager::handleNewTick(TICK current) const {
  std::map<std::string, TickLogger *>::const_iterator i=m_logs.begin(), 
    endi = m_logs.end();
  for( ; endi!=i; ++i)
    i->second->handleNewTick(current);
}

std::string LogManager::file_name(std::string const &short_name) const {
  return m_path+"/"+short_name;
}

std::string const &LogManager::use(std::string const &fileName) {
  std::ifstream src(fileName.c_str(), std::ios::binary);

  if( !!src ) {
    // Create a sub directory for safety :
    int ret = mkdir(LogManager::instance().file_name("cfg").c_str(), 0777);
    if( 0!=ret )
      checkError(EEXIST==errno, "LogManager: "<<strerror(errno));
    std::string 
      destName = LogManager::instance().file_name("cfg/"+short_name(fileName));
    std::ofstream dest(destName.c_str(), std::ios::binary);
    dest<<src.rdbuf();
  } else 
    std::cerr<<"Unable to find \""<<fileName<<'\"'<<std::endl;
  return fileName;
}
