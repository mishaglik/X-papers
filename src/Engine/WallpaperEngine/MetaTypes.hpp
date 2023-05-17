#ifndef ENGINE_WALLPAPERENGINE_METATYPES_HPP
#define ENGINE_WALLPAPERENGINE_METATYPES_HPP

#include <stddef.h>

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
    void* m_data[];
};

#ifdef __cplusplus

template<const MetaType* type>
struct MetaObjectT : MetaObject {
    static constexpr const MetaType* const Type = type;
    MetaObjectT() : MetaObject{.m_type = type} {}
};

}
#endif

#endif /* ENGINE_WALLPAPERENGINE_METATYPES_HPP */
