#ifndef ENGINE_WALLPAPERENGINE_EVENT_HPP
#define ENGINE_WALLPAPERENGINE_EVENT_HPP

#include <concepts>
#include <cstdint>
#include <Utilities/defines.hpp>
namespace xppr {


    struct guid_t {
        byte_t guid[16] = {};
    };

    /**
     * @brief Base class of all events.
     * @warn EVERY child class MUST have staic const guid_t and satisfy Event concept
     */
    class IEvent {
    public:
        /**
         * @brief GUID for event. Used for cast identefication.
         */
        const guid_t guid;

        template<typename T>
        using Property = T IEvent::*; // C++ pointer to member

        IEvent(guid_t guid) : guid(guid) {}
        virtual ~IEvent() {}

        template<typename T>
        T& operator[](Property<T> prop) { return this->*prop; }

        template<typename T>
        const T& operator[](Property<T> prop) const { return this->*prop; }
    };

    //Maybe namespace?
    template<class T>
    concept Event = std::derived_from<T, IEvent> && 
                    requires {T::GUID;} && std::same_as<decltype(T::GUID), guid_t> && requires {T::GUID != 0;};

    template<Event T>
    const T* event_cast(const IEvent* event) {
        if(event->guid == T::GUID) {
            return static_cast<const T*>(event);
        }
        return nullptr;
    }

    template<Event T>
    T* event_cast(IEvent* event) {
        if(event->guid == T::GUID) {
            return static_cast<T* >(event);
        }
        return nullptr;
    }
}

#endif /* ENGINE_WALLPAPERENGINE_EVENT_HPP */
