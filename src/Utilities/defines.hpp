#include "log.hpp"

namespace xppr {
    using byte_t = char;


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

// timestamp: [ERROR] module: 

}
