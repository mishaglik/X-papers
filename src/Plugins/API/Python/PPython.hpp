#ifndef PLUGINS_API_PYTHON_PPYTHON_HPP
#define PLUGINS_API_PYTHON_PPYTHON_HPP

#include <map>

#include <Python.h>

#include <Engine/WallpaperEngine/MetaTypes.hpp>

class PyCharmer {
   public:
    PyCharmer();
    ~PyCharmer() {}

    void init();

    void startScript(const char* filename);

    void registerType(const xppr::meta::MetaType*);

    void addModuleMember(xppr::meta::MetaObject*);

    static PyCharmer* Current;  // FIXME: Make part of python environment

    PyObject* buildPyObject(xppr::meta::MetaObject* object);

    PyObject* getMainObject() { return m_main_object; }

   private:

    xppr::meta::MetaObject m_main_object_instance;

    PyObject* m_main_object;

    std::map<const xppr::meta::MetaType*, PyTypeObject*> m_type_map;

    PyTypeObject* getPyType(const xppr::meta::MetaType* type);
};

#endif /* PLUGINS_API_PYTHON_PPYTHON_HPP */
