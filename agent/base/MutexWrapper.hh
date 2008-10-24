/* -*- C++ -*-
 * $Id$
 */
/** @file "MutexWrapper.hh"
 * @brief Definition of the Mutex class
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#ifndef _MUTEXWRAPPER_HH
# define _MUTEXWRAPPER_HH

# include <pthread.h>

# include "ErrnoExcept.hh"

namespace TREX {

  /** @brief simple mutex implementation.
   *
   * This class provide a simple implementation for mutex.
   *
   * @author Frederic Py <fpy@mbari.org>
   */
  class Mutex {
  public:
    /** @brief Constructor.
     *
     * Create a new mutex instance.
     *
     * @post The mutex is unlocked.
     *
     * @throw ErrnoExcept error during mutex resource creation.
     */
    Mutex();
    /** @brief Destructor.
     *
     * @throw ErrnoExcept error dur
     */
    ~Mutex();
    
    /** @brief mutex lock
     *
     * Lock the current instance. If it is already locked
     * it wait until we can lock it.
     *
     * @post The mutex is locked.
     *
     * @throw ErrnoExcept Error during operation.
     * 
     * @sa Mutex::trylock()
     * @sa Mutex::unlock()
     */
    void lock();
    /** @brief Non blocking mutex lock.
     *
     * Tries to lock the mutex. If the mutex is already locked this method just return immediatly
     * without trying to lock it anymore.
     *
     * @retval true if the mutex was successfully locked
     * @retval false if the mutex is already locked elsewhere.
     *
     * @throw ErrnoExcept porblemn during the operation.
     *
     * @sa Mutex::lock()
     * @sa Mutex::unlock()
     * @sa Mutex::islocked()
     */
    bool trylock();
    
    /** @brief Mutex unlock.
     *
     * This method unlock the mutex.
     *
     * @post mutex is unlocked
     *
     * @throw ErrnoExcept problemn during operation.
     *
     * @sa Mutex::lock()
     * @sa Mutex::trylock()
     */
    void unlock();
    
    /** @brief Check if mutex ius locked
     *
     * This method test if this instance is locked.
     *
     * @retval true The mutex is locked
     * @retval false else
     *
     * @throw ErrnoExcept problem during operation.
     *
     * @sa Mutex::trylock()
     * @sa Mutex::lock()
     * @sa Mutex::unlock()
     */
    bool isLocked() const;
    
  private:
    /** @brief mutex id */
    mutable pthread_mutex_t m_mutexId;

    /** @brief Non blocking lock check.
     *
     * This method is internally used by isLocked() and trylock() methods to
     * make a non blocking lock attempt.
     *
     * @param test Is it just a test. If this argument is true then the function
     * will unlock the mutex just ater a succesfull lock operation.
     * @param from An identifier to know the "real" origin of a possible  exception.
     *
     * @retval true The lock was successfull
     * @retval false The mutex is already locked
     *
     * @throw ErrnoExcept problem during the operation.
     *
     * @sa Mutex::trylock()
     * @sa Mutex::isLocked()
     */
    bool checkLock(bool test, std::string const &from) const;

    // Following functions are not implemented in purpose
    Mutex(Mutex const &);
    void operator= (Mutex const &);
  }; // TREX::Mutex

} // TREX

#endif // _MUTEX_HH
