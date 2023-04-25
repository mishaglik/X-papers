#include "log.hpp"

namespace xppr {
    using byte_t = char;

    template<class T>
    using Vector = std::vector<T>;

#ifndef NDEBUG

#define IFDEBUG(...) __VA_ARGS__

#define THROW(x) throw (x)

#undef assert
#define assert(x) do {                                                                                         \
    if(!(x)) {                                                                                                 \
        xppr::log::critical("{}:{}:{} assertion `{}` failed", __FILE__, __LINE__, __PRETTY_FUNCTION__, #x);    \
        THROW(std::runtime_error("Assertion failed"));                                                         \
    }} while(0)

#else

#define IFDEBUG(...)

#define THROW(x)
#undef assert
#define assert(x)

#endif

#define NOT_IMPLEMENTED xppr::log::critical("{}:{}:{} not implemented", __FILE__, __LINE__, __PRETTY_FUNCTION__); THROW(std::runtime_error("Not implemented"))

// timestamp: [ERROR] module: 

}
