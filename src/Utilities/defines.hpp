#include "log.hpp"

namespace xppr {
    using byte_t = char;


#ifndef NDEBUG

#define IFDEBUG(...) __VA_ARGS__

#undef assert
#define assert(x) (x) ? ((void)0) : ({xppr::log::critical("{}:{}:{} assertion `{}` failed", __FILE__, __LINE__, __PRETTY_FUNCTION__, #x); exit(6);})

#else

#define IFDEBUG(...)

#undef assert
#define assert(x)

#endif

// timestamp: [ERROR] module: 

}