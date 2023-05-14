#include "PaperPy.hpp"
#include "PyCInterface.h"

static xppr::ApplicationAPI AppAPI(nullptr);

static xppr::meta::MetaClass* hello(xppr::meta::ArgPack* ap) {
    printf("Hello %p\n", ap->m_data[0]);
    return nullptr;
}

static xppr::meta::MetaFunction mainMethods[] {
    {"print", "L", hello},
    {"loadModule", "s", load_module},
    {NULL, NULL, NULL}
};


static xppr::meta::MetaClass mainObject {
    "main",
    mainMethods,
    nullptr
};


extern "C" void init_plugin(xppr::ApplicationAPI api) {
    AppAPI = api;
    api.addConnector(new PyConnector);
    init_python_module(&mainObject, "install/paperconfig.py");
}


void PyConnector::registerClass(xppr::meta::MetaClass* meta) {
    reg_class(meta);
}

xppr::meta::MetaClass* load_module(xppr::meta::ArgPack* ap) {
    AppAPI.loadPlugin(reinterpret_cast<char* >(ap->m_data[0]));
    return nullptr;
}
