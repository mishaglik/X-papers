#include "PaperPy.hpp"

extern "C" void init_python_module(xppr::meta::MetaClass* metaclass);

static xppr::meta::MetaClass* hello(xppr::meta::ArgPack* ap) {
    printf("Hello %p\n", ap->m_data[0]);
    return nullptr;
}

static xppr::meta::MetaFunction mainMethods[] {
    {"print", "L", hello},
    {"loadModule", "s", load_module},
    {NULL, NULL, NULL}
};

struct PyMeta : xppr::meta::MetaClass {
    xppr::ApplicationAPI api;
};

static PyMeta mainObject {
    {"main",
    mainMethods,
    nullptr}, nullptr
};


extern "C" void init_plugin(xppr::ApplicationAPI api) {
    mainObject.api = api;
    api.addConnector(new PyConnector);
    init_python_module(&mainObject);
}

extern "C" void reg_class(xppr::meta::MetaClass* meta);

void PyConnector::registerClass(xppr::meta::MetaClass* meta) {
    reg_class(meta);
}

xppr::meta::MetaClass* load_module(xppr::meta::ArgPack* ap) {
    mainObject.api.loadPlugin(reinterpret_cast<char* >(ap->m_data[0]));
    return nullptr;
}
