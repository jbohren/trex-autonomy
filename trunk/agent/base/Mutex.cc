/* -*- C++ -*-
 * $Id$
 */
/** @file "Mutex.cc"
 *
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#include "Mutex.hh"

using namespace TREX;

/*
 * class Mutex
 */
// Structors :

Mutex::Mutex() {
  int ret;

  ret = pthread_mutex_init(&m_mutexId, NULL);
  if( 0!=ret )
    throw ErrnoExcept("Mutex::Mutex");
}

Mutex::~Mutex() {
  int ret;

  ret = pthread_mutex_destroy(&m_mutexId);
  if( 0!=ret )
    throw ErrnoExcept("Mutex::~Mutex");
}

// Manipulators :

void Mutex::lock() {
  int ret;

  ret = pthread_mutex_lock(&m_mutexId);
  if( 0!=ret ) 
    throw ErrnoExcept("Mutex::lock");
}

void Mutex::unlock() {
  int ret;

  ret = pthread_mutex_unlock(&m_mutexId);
  if( 0!=ret ) 
    throw ErrnoExcept("Mutex::unlock");
}

bool Mutex::checkLock(bool test, std::string const &from) const {
  int ret;
  
  ret = pthread_mutex_trylock(&m_mutexId);
  switch(ret) {
  case EBUSY:
    return false;
  case 0:
    if( test )
      ret = pthread_mutex_unlock(&m_mutexId);
    if( ret==0 )
      return true;
  default:
    throw ErrnoExcept(from);
  }
}

bool Mutex::trylock() {
  return checkLock(false, "Mutex::trylock");
}

// Observers :

bool Mutex::isLocked() const {
  return !checkLock(true, "Mutex::isLocked");
}
