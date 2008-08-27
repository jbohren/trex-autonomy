/* -*- C++ -*-
 * $Id$
 */
/** @file "TickLogger.cc"
 *
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#include "LogManager.hh"

using namespace TREX;

/*
 * class TickLogger
 */ 
// structors :

TickLogger::TickLogger(std::string const &fileName)
  :m_baseName(fileName), m_file(LogManager::instance().file_name(fileName).c_str()),
   m_inHeader(true) {}

TickLogger::~TickLogger() {
  std::map<std::string, AbstractField *>::iterator i = m_fields.begin(), 
    endi=m_fields.end();
  for( ; endi!=i; ++i )
    delete i->second;
}

// Manipulators :

TickLogger::AbstractField &TickLogger::getField(std::string const &name) {
  return *(m_fields.find(name)->second);
}

void TickLogger::printHeader() {
  m_inHeader = false;
  order_type::const_iterator i=m_print_order.begin(), endi = m_print_order.end();
  size_t count = 2;
  
  m_file<<"TICK[1]";
  for(; endi!=i; ++i, ++count)
    m_file<<'\t'<<(*i)->first<<'['<<count<<']';
  m_file<<std::endl;
}

// Observers :

void TickLogger::handleNewTick(TICK current) {
  order_type::const_iterator i=m_print_order.begin(), endi = m_print_order.end();
  
  m_file<<current;
  for( ; endi!=i; ++i) {
    m_file.put('\t');
    (*i)->second->print(m_file);
  }
  m_file<<std::endl;
}

bool TickLogger::exist(std::string const &name) const {
  return m_fields.find(name)!=m_fields.end();
}

TickLogger::AbstractField const &TickLogger::getField(std::string const &name) const {
  return *(m_fields.find(name)->second);
}
