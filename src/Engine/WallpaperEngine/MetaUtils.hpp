#ifndef ENGINE_WALLPAPERENGINE_METAUTILS_HPP
#define ENGINE_WALLPAPERENGINE_METAUTILS_HPP
#include <Engine/WallpaperEngine/MetaTypes.hpp>
#include <Utilities/utils.hpp>
#include <concepts>
#include <cstdint>
#include <utility>
#include <Engine/WallpaperEngine/Connector.hpp>

namespace xppr::meta {

template<class T>
concept Meta = std::derived_from<T, MetaObject> && std::same_as<decltype(T::Type), const MetaType* const>;

template<class T>
requires std::derived_from<T, MetaObject>
T* meta_cast(MetaObject* object, const MetaType* type) {
    if(object->m_type == type) {
        return static_cast<T*>(object);
    }
    return nullptr;
}

template<Meta T>
T* meta_cast2(MetaObject* object) {
    if(object->m_type == T::Type) {
        return static_cast<T*>(object);
    }
    return nullptr;
}

namespace detail {


    class MetaLong : public MetaObject {
        public:
        static const constexpr MetaType TypeObj{"Long", nullptr, nullptr, nullptr}; 
        static constexpr const MetaType* Type = &TypeObj;
        uint64_t m_long;
    };

    template<class T>
    concept MetaParametr = 
        std::same_as<T, uint64_t   > ||
        std::same_as<T, const char*> ||
        std::same_as<T, MetaObject*>;


    template<MetaParametr T>
    struct GetChar;

    template<>
    struct GetChar<uint64_t> {
        constexpr static char Get() {return 'L';}
    };

    template<>
    struct GetChar<const char*> {
        constexpr static char Get() {return 's';}
    };

    template<>
    struct GetChar<MetaObject*> {
        constexpr static char Get() {return 'O';}
    };

    template<MetaParametr T>
    struct arg_cast 
    {
        T operator()(void* object) {
            return reinterpret_cast<T>(object);
        }
    };
    


    template<typename... Args>
    bool All(bool arg, Args&&... args) {
        return arg && All(std::forward<Args>(args)...);
    }

    template<>
    inline bool All(bool arg) {
        return arg;
    }

    template<class F>
    struct GetCallback;

    template<Meta T, MetaParametr... Args>
    struct GetCallback<  MetaObject* (T::*)(Args...)> {
        using MethType = MetaObject* (T::*)(Args...);

        template<MethType meth>
        static MetaObject* call(ArgPack* ap) {
            return call_impl<meth>(ap, std::make_index_sequence<sizeof...(Args)>{});
        }  

    private:
        template<MethType Meth, std::size_t... Idx>
        static MetaObject* call_impl(ArgPack* ap, std::index_sequence<Idx...>) {
            if(T* self = meta_cast<T>(ap->self, T::Type)) {
                
                // if(!All(arg_check<Args>(ap->m_data[Idx], ap->m_signature[Idx])...)) {
                //     return nullptr;
                // }

                return (self->*Meth)(arg_cast<Args>{}(ap->m_data[Idx])...);
            }
            return nullptr;
        }
    public:
        constexpr static const char signature[sizeof...(Args) + 1] = {GetChar<Args>::Get()...}; 
    };

    template<Meta T, MetaParametr... Args>
    struct GetCallback<  void (T::*)(Args...)> {
        using MethType = void (T::*)(Args...);

        template<MethType meth>
        static MetaObject* call(ArgPack* ap) {
            return call_impl<meth>(ap, std::make_index_sequence<sizeof...(Args)>{});
        }  

    private:
        template<MethType Meth, std::size_t... Idx>
        static MetaObject* call_impl(ArgPack* ap, std::index_sequence<Idx...>) {
            if(T* self = meta_cast2<T>(ap->self)) {
                
                // if(!All(arg_check<Args>(ap->m_data[Idx], ap->m_signature[Idx])...)) {
                //     return nullptr;
                // }

                (self->*Meth)(arg_cast<Args>{}(ap->m_data[Idx])...);
            }
            return nullptr;
        }
    public:
        constexpr static const char signature[sizeof...(Args) + 1] = {GetChar<Args>::Get()...}; 
    };

}

}
#define META_METHOD(type, name) \
        {#name, xppr::meta::detail::GetCallback<decltype(&type::name)>::signature, xppr::meta::detail::GetCallback<decltype(&type::name)>::call<&type::name>}



// template<class T, const char* name>
// extern const xppr::meta::MetaType MetaMgrType;

// template<class T, const char* name>
// struct MetaMgr : xppr::meta::MetaObjectT<&MetaMgrType<T, name>> {
//     xppr::ApplicationAPI m_api;
//     xppr::meta::MetaObject* add(uint64_t i) {
//         auto wid = new T();
//         m_api.addWidget(wid, i);
//         return wid;
//     }
// };

// template<class T, const char* name>
// const xppr::meta::MetaFuction MetaMgrMeths[] = {
//     {"add", xppr::meta::detail::GetCallback<decltype(&(MetaMgr<T, name>::add))>::signature, xppr::meta::detail::GetCallback<decltype(&(MetaMgr<T, name>::add))>::template call<&(MetaMgr<T, name>::add)>},
//     {nullptr, nullptr}
// };

// template<class T, const char* name>
// const xppr::meta::MetaType MetaMgrType
// {
//     .name = name,
//     .methods = MetaMgrMeths<T, name>,
//     .members = nullptr,
//     .dtor = nullptr
// };


#endif /* ENGINE_WALLPAPERENGINE_METAUTILS_HPP */
