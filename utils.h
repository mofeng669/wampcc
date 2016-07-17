#ifndef XXX_UTILS_H
#define XXX_UTILS_H

#include "logger.h"

#include <sstream>
#include <list>


#define THROW(E, X )  do                        \
  {                                             \
    std::ostringstream __os;                    \
    __os << X ;                                 \
    throw E ( __os.str() );                     \
  } while(false);


namespace XXX {

class logger;

enum class HMACSHA256_Mode
{
  HEX,
  BASE64
};

int compute_HMACSHA256(const char* key,
                       int keylen,
                       const char* msg,
                       int msglen,
                       char * dest,
                       unsigned int * destlen,
                       HMACSHA256_Mode output_mode);


/* must be called with an active exception */
void log_exception(logger *__logptr, const char* callsite);


/* Implements a general list of observers, which can be notified with a generic
 * function with variadic arguments. Observer objects should be plain structs
 * that consist of a set of std::function members. */
template<typename T>
class observer_list
{
public:
  struct key {};

  /* Add observer, returning a unique key used for later removal. */
  key* add(T&& obs)
  {
    m_observers.emplace_back(key(), std::move(obs));
    return &m_observers.back().first;
  }


  /* Notify observers, by applying a functional object, with supplied
   * arguments. */
  template<typename F, typename... Args>
  void notify(const F& fn, Args&&... args)
  {
    for (auto & item : m_observers)
      fn( item.second, args... );
  }

private:
  std::list< std::pair<key, T>  > m_observers;
};

} // namespace XXX

#endif
