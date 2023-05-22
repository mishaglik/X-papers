#include "PaperPy.hpp"
#include "PPython.hpp"
#include <Utilities/utils.hpp>

PyConnector* Connector = nullptr; 

static xppr::meta::MetaObject* hello(xppr::meta::ArgPack* ap) {
    printf("Hello %p\n", &ap->m_args[0]);
    return nullptr;
}

static xppr::meta::MetaObject* loadModule(xppr::meta::ArgPack* ap) {
    Connector->m_app.loadPlugin(std::get<std::string>(ap->m_args[0]).c_str());
    return nullptr;
}

static xppr::meta::MetaFuction mainMethods[] {
    {"print", "L", hello},
    {"loadModule", "s", loadModule},
    {nullptr, nullptr, nullptr}
};


xppr::meta::MetaType MainObjectType {
    "main",
    mainMethods,
    nullptr
};

extern "C" void init_plugin(xppr::ApplicationAPI api) {
    xppr::log::add_logger();
    PyCharmer::Current = new PyCharmer;
    PyCharmer::Current->init();
    Connector = new PyConnector(api, PyCharmer::Current);
    api.addConnector(Connector);
    Connector->m_charmer->registerType(&MainObjectType);
    Connector->m_charmer->startScript("install/paperconfig.py");
}

PyConnector::~PyConnector() {
    delete m_charmer;
    PyCharmer::Current = nullptr;
}


void PyConnector::registerObject(xppr::meta::MetaObject* meta) {
    m_charmer->addModuleMember(meta);
}


void PyConnector::registerClass(xppr::meta::MetaType* meta) {
    m_charmer->registerType(meta);
}

