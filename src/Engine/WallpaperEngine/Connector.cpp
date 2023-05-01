#include "Connector.hpp"
#include "WallpaperEngine.hpp"
#include <Utilities/utils.hpp>
namespace xppr {

APIError ApplicationAPI::addWidget(WidgetBase*) {
    NOT_IMPLEMENTED;
}

APIError ApplicationAPI::addConnector(ConnectorBase* cb) {
    return APIError{m_wallpaper->addConnector(cb)};
}

APIError ApplicationAPI::loadPlugin(const char* filename) {
    return APIError{m_wallpaper->loadPlugin(filename)};
}

}
