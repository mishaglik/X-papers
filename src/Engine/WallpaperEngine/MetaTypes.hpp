#ifndef ENGINE_WALLPAPERENGINE_METATYPES_HPP
#define ENGINE_WALLPAPERENGINE_METATYPES_HPP

#include <stddef.h>
#include <cstdint>
#include <variant>
#include <vector>
#include <string>

#ifdef __cplusplus
namespace xppr::meta {
#endif


struct MetaType;
struct MetaObject;
struct ArgPack;

#ifndef __cplusplus
typedef struct MetaType MetaType;
typedef struct MetaObject MetaObject;
typedef struct ArgPack ArgPack;
#endif

typedef MetaObject* (*MetaCallback)(ArgPack* args);

struct MetaFuction {
    const char* name;
    const char* signature;
    MetaCallback callback;
};

enum MetaTypes {
    Void,
    Long,
    String,
    Object,
    Array,
};

template<class T>
struct CArray {
    T* data;
    size_t size;
};

struct MetaTyped : std::variant<uint64_t, std::string, MetaObject*, std::vector<MetaTyped>> {
    MetaTyped() = default;

    MetaTyped& operator=(uint64_t obj) {
        Variant::operator=(obj);
        return *this;
    }

    MetaTyped& operator=(int64_t obj) {
        return *this = static_cast<uint64_t>(obj);
    }

    template<class T> 
    MetaTyped& operator=(T obj) {
        Variant::operator=(std::forward<T>(obj));
        return *this;
    }

    private: 
        using Variant = std::variant<uint64_t, std::string, MetaObject*, std::vector<MetaTyped>>;
};

union MetaTypedEx {
    uint64_t    m_long;
    const char* m_string;
    MetaObject* m_object;
    CArray<MetaTypedEx> m_array;
};

struct MetaMember {
    const char* name;
    enum MetaTypes type;
    void* MetaObject::*offset;
    bool writable;
};

#ifndef __cplusplus
typedef struct MetaFuction MetaFuction;
typedef struct MetaMember MetaMember;
#endif

struct MetaType {
    const char* name;
    const MetaFuction* methods;
    const MetaMember* members;
    void (*dtor)();
};

struct MetaObject {
    const MetaType* m_type;
};

struct ArgPack {
    const char* m_signature;
    MetaObject* self;
    std::vector<MetaTyped> m_args;
};

extern const MetaType ErrorType;

#ifdef __cplusplus


template<const MetaType* type>
struct MetaObjectT : MetaObject {
    static constexpr const MetaType* const Type = type;
    MetaObjectT() : MetaObject{.m_type = type} {}
};

struct MetaError : public MetaObjectT<&ErrorType> {
    enum class Type {
        TypeError,
        AttributeError,
        OtherError
    };
    Type m_err_type;
    const char* m_message;
};
}

#endif

#endif /* ENGINE_WALLPAPERENGINE_METATYPES_HPP */
