#include <Python.h>
#include <Engine/WallpaperEngine/Connector.hpp>
#include <map>

extern "C" void init_plugin(xppr::ApplicationAPI api);

class PyCharmer;
class PyConnector final : public xppr::ConnectorBase {
   public:
    explicit PyConnector(xppr::ApplicationAPI api, PyCharmer* charmer)
        : m_app(api), m_charmer(charmer) {}

    virtual void update(uint64_t) override {}
    
    virtual void registerClass(xppr::meta::MetaType* meta) override;
    
    virtual void registerObject(xppr::meta::MetaObject* meta) override;
    
    virtual ~PyConnector() override;
    
    xppr::ApplicationAPI m_app;
    
    PyCharmer* m_charmer;
};

extern PyConnector* Connector;
extern xppr::meta::MetaType MainObjectType;

// xppr::meta::MetaObject* load_module(xppr::meta::ArgPack* ap);
