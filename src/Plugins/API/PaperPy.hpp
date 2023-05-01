#include <Engine/WallpaperEngine/Connector.hpp>

extern "C" void init_plugin(xppr::ApplicationAPI api);

class PyConnector final : public xppr::ConnectorBase {
public:
    virtual void update(uint64_t) override {}
    virtual void registerClass(xppr::meta::MetaClass* meta) override;
    virtual ~PyConnector() override {}
};

xppr::meta::MetaClass* load_module(xppr::meta::ArgPack* ap);