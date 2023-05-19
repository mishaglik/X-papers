#include <Engine/WallpaperEngine/Connector.hpp>
#include <Engine/WallpaperEngine/MetaUtils.hpp>
#include <Utilities/utils.hpp>
#include "Plugins/Widget/BaseWidgets/widgets/Label.hpp"
#include "SpriteWidget.hpp"
#include "Sprited.hpp"
#include "decor/Border.hpp"

struct BaseWidgetsMgr;

extern const xppr::meta::MetaFuction BaseWidgetsMgrMeths[];

static const xppr::meta::MetaType BaseWidgetsMgrType{
    .name = "wid",
    .methods = BaseWidgetsMgrMeths,
    .members = nullptr,
    .dtor = nullptr,
};

struct BaseWidgetsMgr : xppr::meta::MetaObjectT<&BaseWidgetsMgrType> {
    xppr::ApplicationAPI m_api;

    template<typename T>
    xppr::meta::MetaObject* add(uint64_t i) {
        if(i >= m_api.NDisplays()) {
            return new xppr::meta::MetaError{{}, xppr::meta::MetaError::Type::AttributeError, "Index of display is too big!"};
        }
        auto* wid = new T(100, 100);
        m_api.addWidget(wid, i);
        return wid;
    }

    xppr::meta::MetaObject* addBorderedLabel(uint64_t i) {
        if(i >= m_api.NDisplays()) {
            return new xppr::meta::MetaError{{}, xppr::meta::MetaError::Type::AttributeError, "Index of display is too big!"};
        }
        auto* wid = Label() | Bordered(0x00FF00FF) | OnSprite(200, 30);
        m_api.addWidget(wid, i);
        return wid;
    }
};

#define ADD_(T) {"add" #T, META_SIGNATURE(&BaseWidgetsMgr::add<T>), META_CALLBACK(&BaseWidgetsMgr::add<T>)}
const xppr::meta::MetaFuction BaseWidgetsMgrMeths[] = {
    // ADD_(Lab),
    META_METHOD(BaseWidgetsMgr, addBorderedLabel),
    {}
};
#undef ADD_

extern "C" void init_plugin(xppr::ApplicationAPI api) {
    xppr::log::add_logger();
    api.registerObject(new BaseWidgetsMgr{{}, api});
}
