#ifndef ENGINE_WALLPAPERENGINE_METADEFAULTS_HPP
#define ENGINE_WALLPAPERENGINE_METADEFAULTS_HPP

#include <Engine/WallpaperEngine/MetaClass.hpp>
#include "Engine/WallpaperEngine/Connector.hpp"
namespace xppr::meta {

    struct MetaPluginClassBase : MetaClass {
        ApplicationAPI m_api;
    };

    template<class T>
    MetaClass* metaNew(ArgPack* ap) {
        auto* widget = new T();
        reinterpret_cast<MetaPluginClassBase* >(ap->self)->m_api.addWidget(widget, reinterpret_cast<uint64_t>(ap->m_data[0]));
        return widget;
    }

    template<class T>
    const MetaFunction PMetods[] = {
        {"add", "L", metaNew<T>},
        {nullptr, nullptr, nullptr}
    };

    template<class T, const char* name>
    MetaPluginClassBase PMetaClass = {
        {name,
        PMetods<T>,
        nullptr}, nullptr
    };

    template<class T, class U>
    using Property = U T::*;

    template<class T>
    concept Parameter = std::same_as<T, MetaClass*> || std::same_as<T, const char*> || std::same_as<T, void*> || std::same_as<T, uint64_t>;

    template<class T, Parameter ...Args, void (T::*Meth)(Args...), std::size_t... Idx>
    void call_impl(ArgPack* ap, std::index_sequence<Idx...>) {
        static_cast<T* >(ap->self)->*Meth(reinterpret_cast<Args>(ap->m_data[Idx])...);
    }
   
    template<class T, class F, F f, Parameter ...Args>
    MetaClass* call(ArgPack* ap) {
        call_impl<T, F, f, Args...>(ap, std::make_index_sequence<sizeof...(Args)>{});
        return nullptr;
    }

    template <class T>
    struct GetChar;

    template<>
    struct GetChar<const char* > {
        static constexpr const char sym = 's';
    };

    template<>
    struct GetChar<const std::string&> {
        static constexpr const char sym = 's';
    };

    template<>
    struct GetChar<uint64_t> {
        static constexpr const char sym = 'L';
    };

    template<class F>
    struct GetCB;

    template<class T, Parameter... Args>
    struct GetCB<void (T::*)(Args...)> {
        // static constexpr MetaCallback getCallback() {
            // return call<T, Args..., Meth>;
        // }
        using MethType = void (T::*)(Args...);
        template<MethType Meth>
        static MetaClass* call(ArgPack* ap) {
            return call_impl<Meth>(ap, std::make_index_sequence<sizeof...(Args)>{});
        }

        template<MethType Meth, std::size_t... Idx>
        static MetaClass* call_impl(ArgPack* ap, std::index_sequence<Idx...>) {
            (static_cast<T* >(ap->self)->*Meth)(reinterpret_cast<Args>(ap->m_data[Idx])...);
            return nullptr;
        }

        static constexpr char signature[sizeof...(Args) + 1] = {GetChar<Args>::sym...};
    };

    #define META_METHOD(Type, Method) \
        {#Method, xppr::meta::GetCB<decltype(&Type::Method)>::signature, xppr::meta::GetCB<decltype(&Type::Method)>::call<&Type::Method>}

}


#endif /* ENGINE_WALLPAPERENGINE_METADEFAULTS_HPP */
