#if 0
#ifndef ENGINE_WALLPAPERENGINE_METACLASS_HPP
#define ENGINE_WALLPAPERENGINE_METACLASS_HPP

#ifdef __cplusplus
namespace xppr::meta {
#endif


struct ArgPack {
    const char* m_signature;
    struct MetaClass* self;
    void* m_data[];
};

struct MetaClass;

#ifndef __cplusplus
typedef struct ArgPack ArgPack;
typedef struct MetaClass MetaClass;
#endif

struct MetaFunction {
    const char* m_name;
    const char* m_signature;
    MetaClass* (*m_callback)(ArgPack* ap);
};

typedef MetaClass* (*MetaCallback)(ArgPack* ap);

struct MetaMember {
    const char* m_name;
    char type;
    void* data;
};

#ifndef __cplusplus
typedef struct MetaFunction MetaFunction;
typedef struct MetaMember MetaMember;
#endif

struct MetaClass {
    const char* m_name;
    const MetaFunction* m_methods; // NULL terminated
    const MetaMember*   m_members; // NULL terminated
};

typedef struct MetaClass MetaClass;

#ifdef __cplusplus
} // namespace xppr::meta
#endif
#endif /* ENGINE_WALLPAPERENGINE_METACLASS_HPP */
#endif