/* -*- C++ -*-
 * $Id$
 */
/** @file "Guardian.hh"
 * @brief defintion of Guardian class
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#ifndef _GUARDIAN_HH
# define _GUARDIAN_HH

namespace TREX {

  /** @brief Critical section facility.
   *
   * This class is used to ensure the locking/unlocking of a
   * mutex like class in a given context. The locking is done
   * during construction and the unlocking during destruction
   * so we ensure that a locked instance will be unlocked when
   * exiting current context even if an exception is thrown.
   *
   * @param Mtx The type of the managedd mutex like class.
   *
   * @author Frederic Py <fpy@mbari.org>
   */
  template<class Mtx>
  class Guardian {
  public:
    /** @brief Constructor.
     *
     * @param guarded A mutex like instance.
     *
     * Lock @e guarded
     *
     * @post @e guarded is locked 
     */
    Guardian(Mtx &guarded)
      :m_guarded(guarded) {
      m_guarded.lock();
    }

    /** @brief Destructor.
     *
     * Unlock the guarded mutex like instance.
     */
    ~Guardian() {
      m_guarded.unlock();
    }  
  private:
    /** brief Guarded mutex like instance. */
    Mtx &m_guarded;

    // Following functions have no code in purpose
    Guardian(Guardian const &);
    void operator= (Guardian const &);
  }; // TREX::Guardian

} // TREX

#endif // _GUARDIAN_HH
