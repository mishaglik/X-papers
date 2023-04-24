#ifndef ENGINE_WALLPAPERENGINE_EVENT_HPP
#define ENGINE_WALLPAPERENGINE_EVENT_HPP

#include <concepts>
#include <cstdint>
#include <Utilities/defines.hpp>
#include "MetaObject.hpp"
namespace xppr {

    
    class IEvent : public MetaObject{
      public:
      IEvent(guid_t guid) : MetaObject(guid) {}
    };

}

#endif /* ENGINE_WALLPAPERENGINE_EVENT_HPP */