#ifndef ENGINE_WALLPAPERENGINE_METADEFAULTS_HPP
#define ENGINE_WALLPAPERENGINE_METADEFAULTS_HPP

#include <WallpaperEngine/MetaClass.hpp>
#include "WallpaperEngine/Connector.hpp"
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

}

#endif /* ENGINE_WALLPAPERENGINE_METADEFAULTS_HPP */
