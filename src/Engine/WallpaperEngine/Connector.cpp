#include "Connector.hpp"
#include "WallpaperEngine.hpp"
#include <Utilities/utils.hpp>
namespace xppr {

APIError ApplicationAPI::addWidget(WidgetBase* widget, uint64_t display) {
    return APIError{m_wallpaper->addWidget(widget, display)};
}

APIError ApplicationAPI::addConnector(ConnectorBase* cb) {
    return APIError{m_wallpaper->addConnector(cb)};
}

APIError ApplicationAPI::loadPlugin(const char* filename) {
    return APIError{m_wallpaper->loadPlugin(filename)};
}

APIError ApplicationAPI::registerClass(xppr::meta::MetaClass* meta) {
    return APIError{m_wallpaper->registerClass(meta)};
}

}
