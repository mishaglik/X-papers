#ifndef ENGINE_WALLPAPERENGINE_METATYPES_HPP
#define ENGINE_WALLPAPERENGINE_METATYPES_HPP

#include <cstdint>
#include <stddef.h>
#include <string>
#include <variant>
#include <vector>

namespace xppr::meta {

struct MetaType;
struct MetaObject;
struct ArgPack;

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

struct MetaMember {
    const char* name;
    enum MetaTypes type;
    void* MetaObject::*offset;
    bool writable;
};


struct MetaType {
    const char* name;
    const MetaFuction* methods;
    const MetaMember* members;
    void (*dtor)(MetaObject* self);
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

#endif /* ENGINE_WALLPAPERENGINE_METATYPES_HPP */
