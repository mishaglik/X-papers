#include "Connector.hpp"
#include "WallpaperEngine.hpp"
#include <Utilities/utils.hpp>
namespace xppr {

APIError ApplicationAPI::addWidget(WidgetBase*) {
    NOT_IMPLEMENTED;
}

APIError ApplicationAPI::addConnector(ConnectorBase*) {
    NOT_IMPLEMENTED;
}

APIError ApplicationAPI::loadPlugin(const char* filename) {
    return APIError{m_wallpaper->loadPlugin(filename)};
}

}
