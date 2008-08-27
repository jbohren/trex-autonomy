/* -*- C++ -*-
 * $Id$
 */
/** @file "TextLog.cc"
 *
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#include "Guardian.hh"

#include "TextLog.hh"

using namespace TREX;
/*
 * class TextLog
 */ 

// structors :

TextLog::TextLog() {}

TextLog::TextLog(std::string const &name)
  :m_log(name.c_str()) {}

TextLog::~TextLog() {}

// Manipulators :

void TextLog::open(std::string const &name) {
  Guardian<Mutex> guard(m_lock);
  
  m_log.open(name.c_str());
}

void TextLog::write(std::string const &text) {
  Guardian<Mutex> guard(m_lock);
  
  m_log<<text<<std::flush;
}

/* 
 * class LogEntry 
 */ 

// structors :

LogEntry::LogEntry(TextLog &owner)
  :m_owner(owner) {}

LogEntry::LogEntry(LogEntry const &other) 
  :m_owner(other.m_owner), m_buff(other.m_buff.str()) {
  other.m_buff.str(""); // cleaning old temporary variable
}

LogEntry::~LogEntry() {
  std::string to_log = m_buff.str();
  if( !to_log.empty() )
    m_owner.write(to_log);
}
