#ifndef PLUGINS_API_PYCINTERFACE_H
#define PLUGINS_API_PYCINTERFACE_H

#include <Engine/WallpaperEngine/MetaClass.hpp>
#ifdef __cplusplus
using namespace xppr::meta;
extern "C" {
#endif

void init_python_module(MetaClass* metaclass, const char* filename);
void reg_class(MetaClass* meta);

#ifdef __cplusplus
}
#endif

#endif /* PLUGINS_API_PYCINTERFACE_H */
