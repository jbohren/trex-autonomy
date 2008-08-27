/* -*- C++ -*-
 */
/** @file "ErrnoExcept.hh"
 *
 * @brief ErrnoExcept definition.
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#ifndef H_ErrnoExcept
# define H_ErrnoExcept

# include <cerrno>

# include <stdexcept>

namespace TREX {

  /** @brief errno based exception.
   *
   * Thios exception may be used to manage exceptional
   * events that modifies errno value.
   *
   * @author Frederic Py <fpy@mbari.org>
   */
  class ErrnoExcept :public std::runtime_error {
  public:
    /** @brief Constructor
     *
     * @param from origin of the error.
     *
     * Create a new instance. The message attached to this instance
     * will have the form : "from: @\"errno message@\"".
     */
    explicit ErrnoExcept(std::string const &from) throw()
      :std::runtime_error(build_message(from)), _errno(errno) {}
    ErrnoExcept(std::string const &from, std::string const &what) throw()
      :std::runtime_error(build_message(from, what)), _errno(0) {}
    /** @brief Destructor */
    virtual ~ErrnoExcept() throw() {}

    /** @brief Get errno value.
     *
     * @return the value of @c errno when the exception was created.
     *
     * This method may be usefull when one want to check if the error
     * was critical or not.
     */
    int get_errno() const {
      return _errno;
    }
    
  private:
    /** @brief value of @c errno when exception was created. */ 
    int _errno;

    static std::string build_message(std::string const &from, 
				     std::string const &what) throw();

    /** @brief Build mnessage content.
     *
     * This method build the message content taking the string
     * message attached to current errno.
     *
     * @return The message built containing error infromation
     * grabbed with @c errno value 
     */
    static std::string build_message(std::string const &from) throw();
  }; // ErrnoExcept

} // TREX

#endif // H_ErrnoExcept
