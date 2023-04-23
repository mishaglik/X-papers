#ifndef ENGINE_WALLPAPERENGINE_METAOBJECT_HPP
#define ENGINE_WALLPAPERENGINE_METAOBJECT_HPP
#include <Utilities/utils.hpp>
namespace xppr {

struct guid_t {
    byte_t guid[16] = {};
};

/**
 * @brief Base class of all metaobjects.
 * @warn EVERY child class MUST have staic const guid_t and satisfy Meta concept
 */
class MetaObject {
public:
    const guid_t m_guid;


    template<typename T>
    using Property = T MetaObject::*; // C++ pointer to member

    MetaObject(guid_t guid) : m_guid(guid) {}
    virtual ~MetaObject() {}

    template<typename T>
    T& operator[](Property<T> prop) { return this->*prop; }

    template<typename T>
    const T& operator[](Property<T> prop) const { return this->*prop; }
};

//Maybe namespace?
    template<class T>
    concept Meta = std::derived_from<T, MetaObject> && 
                    requires {T::GUID;} && std::same_as<decltype(T::GUID), guid_t> && requires {T::GUID != 0;};

    template<Meta T>
    const T* meta_cast(const MetaObject* meta) {
        if(meta->m_guid == T::GUID) {
            return static_cast<const T*>(meta);
        }
        return nullptr;
    }

    template<Meta T>
    T* meta_cast(MetaObject* meta) {
        if(meta->m_guid == T::GUID) {
            return static_cast<T* >(meta);
        }
        return nullptr;
    }

}
#endif /* ENGINE_WALLPAPERENGINE_METAOBJECT_HPP */
