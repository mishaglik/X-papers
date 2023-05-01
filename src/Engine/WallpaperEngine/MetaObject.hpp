#ifndef ENGINE_WALLPAPERENGINE_METAOBJECT_HPP
#define ENGINE_WALLPAPERENGINE_METAOBJECT_HPP
#include <concepts>
#include <cstdint>
#include <string>
#include <vector>
namespace xppr {

struct guid_t {
    char guid[16] = {};
    static constexpr guid_t fromFStr(const char str[37]);
};

/**
 * @brief Builds guid_t from formatted str with format "%8lx-%4lx-%4lx-%4lx-%12lx" ("00112233-4455-6677-8899-AABBCCDDEEFF");
 * 
 * @param str - string  
 * @return constexpr guid_t.
 */
constexpr guid_t guid_t::fromFStr(const char str[37]) {
    guid_t guid;

    /**
     * @brief Supporting lambda to transform 2 hex digits to byte.
     */
    auto byteFromHex = [](const char hex[2]) constexpr {
        char ans = 0;
        if(hex[0] >= '0' &&  hex[0] <= '9') 
            ans |= hex[0] - '0';
        else if(hex[0] >= 'a' &&  hex[0] <= 'f')
            ans |= hex[0] - 'a';
        else if(hex[0] >= 'A' &&  hex[0] <= 'F')
            ans |= hex[0] - 'A';
        else abort();
        ans <<= 4;
        if(hex[0] >= '0' &&  hex[0] <= '9') 
            ans |= hex[0] - '0';
        else if(hex[0] >= 'a' &&  hex[0] <= 'f')
            ans |= hex[0] - 'a';
        else if(hex[0] >= 'A' &&  hex[0] <= 'F')
            ans |= hex[0] - 'A';
        else abort();
        return ans;
    };

    guid.guid[ 0] = byteFromHex(&str[ 0]); // 00 
    guid.guid[ 1] = byteFromHex(&str[ 2]); // 11 
    guid.guid[ 2] = byteFromHex(&str[ 4]); // 22 
    guid.guid[ 3] = byteFromHex(&str[ 6]); // 33 
                                           // -
    guid.guid[ 4] = byteFromHex(&str[ 9]); // 44
    guid.guid[ 5] = byteFromHex(&str[11]); // 55
                                           // -
    guid.guid[ 6] = byteFromHex(&str[14]); // 66
    guid.guid[ 7] = byteFromHex(&str[16]); // 77
                                           // -
    guid.guid[ 8] = byteFromHex(&str[19]); // 88
    guid.guid[ 9] = byteFromHex(&str[21]); // 99
                                           // -
    guid.guid[10] = byteFromHex(&str[24]); // AA
    guid.guid[11] = byteFromHex(&str[26]); // BB
    guid.guid[12] = byteFromHex(&str[28]); // CC
    guid.guid[13] = byteFromHex(&str[30]); // DD
    guid.guid[14] = byteFromHex(&str[32]); // EE
    guid.guid[15] = byteFromHex(&str[34]); // FF
    return guid;
}

/**
 * @brief Base class of all metaobjects.
 * EVERY child class MUST have staic const guid_t and satisfy Meta concept
 */
class MetaObject {
public:
    const guid_t m_guid;


    template<typename T>
    using Property = T MetaObject::*; // C++ pointer to member

    MetaObject(guid_t guid) : m_guid(guid) {}
    virtual ~MetaObject() = 0;

    template<typename T>
    T& operator[](Property<T> prop) { return this->*prop; }

    template<typename T>
    const T& operator[](Property<T> prop) const { return this->*prop; }
};


namespace meta {

    template<class T>
    concept Metatype = std::same_as<T, uint64_t> || std::same_as<T, void* > || std::same_as<T, char*> || 
        (std::is_pointer<T>::is_pointer && std::derived_from<typename std::remove_pointer<T>::type, MetaObject>);

    enum MetatypeFmt : char {
        Int = 'L',
        Str = 's',
        Buf = 'z',
        Obj = 'O',
    };


    struct ArgPack {
        const char* m_signature;
        struct MetaClass* self;
        void* m_data[];
    };

    struct MetaClass;

    struct MetaFunction {
        const char* m_name;
        const char* m_signature;
        MetaClass* (*m_callback)(ArgPack* ap);
    };

    struct MetaMember {
        const char* m_name;
        char type;
        void* data;
    };

    template<class T>
    int ArgPackCall(T t, ArgPack* ap);

    template<Metatype... Args, size_t... Idx>
    int ArgPackCallImpl(int(*f)(Args...), ArgPack* ap, std::index_sequence<Idx...> = std::index_sequence_for<Args...>{}) {
        return f(reinterpret_cast<Args>(ap->m_data[Idx])...);
    }

    template<Metatype... Args>
    int ArgPackCall(int(*f)(Args...), ArgPack* ap) {
        return ArgPackCallImpl(f, ap);
    }

    struct MetaClass {
        const char* m_name;
        MetaFunction* m_methods; // NULL terminated
        MetaMember*   m_members; // NULL terminated
    };

};

namespace rebind {

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
}
#endif /* ENGINE_WALLPAPERENGINE_METAOBJECT_HPP */
