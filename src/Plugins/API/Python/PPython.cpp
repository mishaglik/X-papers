#include "Python.h"
#include "structmember.h"
#include "PPython.hpp"
#include "PaperPy.hpp"
#include <Utilities/log.hpp>

using namespace xppr::meta;

PyCharmer* PyCharmer::Current = nullptr;

static PyObject* XPapersConnect(PyObject* self, PyObject* args);

static PyMethodDef XPapersModuleMethods[] = {
    {"connect", XPapersConnect, METH_NOARGS, "connect"},
    {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef XPapersPyModuleDef = {
    PyModuleDef_HEAD_INIT, 
    "xpapers", 
    "Xpapers module", 
    -1, 
    XPapersModuleMethods, 
    nullptr, 
    nullptr, 
    nullptr, 
    nullptr
};

PyMODINIT_FUNC PyInit__xpapers(); 


PyTypeObject* PyCharmer::getPyType(const MetaType* type) {
    if(PyTypeObject* object = m_type_map[type]) {
        return object;
    }
    registerType(type);
    return m_type_map[type];
}

struct MetaPyMethodObject {
    PyObject_HEAD
    MetaObject* self;
    const MetaFuction* func;
};

static PyObject* MetaPyMethodCall(PyObject* self, PyObject* args, PyObject* kwargs);

static PyTypeObject MetaPyMethodObjectType {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "xpapers.py_meta_method",
    .tp_basicsize = sizeof(MetaPyMethodObject),
    .tp_call = MetaPyMethodCall,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IMMUTABLETYPE,
    .tp_new = PyType_GenericNew,
};

struct MetaPyObject {
    PyObject_HEAD
    MetaObject* self;
    PyObject* dict;
    MetaPyMethodObject* methods[];
};

static const PyTypeObject DefaultMetaType {
    .ob_base =  PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "xpapers.py_meta_class",
    .tp_basicsize = sizeof(MetaPyObject),        
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = "py_meta_class objects",
    .tp_dictoffset = offsetof(MetaPyObject, dict),
    .tp_new = PyType_GenericNew,
};

template<class T>
size_t CArrSize(T* data) {
    if(data == nullptr) {
        return 0;
    }
    size_t i = 1;
    while(data[0].name != nullptr) {
        data++;
        i++;
    }
    return i;
}


static PyObject* MetaGetter(PyObject*, void*);
static int MetaSetter(PyObject*, PyObject*, void*);

void PyCharmer::registerType(const xppr::meta::MetaType* type) {

    assert(DefaultMetaType.tp_dict == nullptr);
    PyTypeObject* type_object = new PyTypeObject(DefaultMetaType); //TODO: Null check

    // size_t n_members = CArrSize(type->members);
    // if(n_members != 0) {
    //     PyGetSetDef* members = new PyGetSetDef[n_members]; //TODO: Null check
    //     for(size_t i = 0; i < n_members-1; ++i) {
    //         members[i] = {
    //             .name = type->members[i].name,
    //             .get = MetaGetter,
    //             .set = type->members[i].writable ? MetaSetter : nullptr,
    //             .doc = "Doc",
    //             .closure = const_cast<void*>(reinterpret_cast<const void*>(&type->members[i])),
    //         };
    //     }
    //     members[n_members-1] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    //     type_object->tp_getset = members;
    // }

    size_t n_methods = CArrSize(type->methods);
    if(n_methods != 0) {
        // PyMemberDef* methods = new PyMemberDef[n_methods]; //TODO: Null check
        // for(size_t i = 0; i < n_methods-1; ++i) {
        //     methods[i] = {
        //         .name = type->methods[i].name,
        //         .type = T_OBJECT_EX,
        //         .offset = static_cast<Py_ssize_t>(offsetof(MetaPyObject, methods) + i * sizeof(void* )),
        //         .flags = READONLY,
        //         .doc = "Doc",
        //     };
        // }
        // 
        // methods[n_methods-1] = {nullptr, 0, 0, 0, nullptr};
        // type_object->tp_members = methods;
        // type_object->tp_basicsize += (n_methods-1) * sizeof(MetaPyMethodObject* ); // Array of pointers MetaPyMethodObject* methods[];
    }

    if(PyType_Ready(type_object) < 0) {
        xppr::log::error("Creating PyTypeObject for {} failed.", type->name);
        delete type_object->tp_members;
        delete type_object->tp_getset;
        delete type_object;
        //FIXME: Pass error
    }
    Py_INCREF(type_object);

    m_type_map[type] = type_object;
}


void PyCharmer::init() {
    PyImport_AppendInittab("xpapers", &PyInit__xpapers);
    Py_Initialize();
    m_main_object = buildPyObject(&m_main_object_impl);
    Py_INCREF(m_main_object);
}

void PyCharmer::startScript(const char* filename) {
    // TODO: Err check
    init();
    PyRun_SimpleFileEx(fopen(filename, "r"), filename, 1);
    Py_FinalizeEx();
}

static PyObject* MetaGetter(PyObject* self, void* closure) {
    if(self == nullptr || closure == nullptr) {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    const MetaMember* member = reinterpret_cast<MetaMember*>(closure);
    MetaObject* object = reinterpret_cast<MetaPyObject*>(self)->self;
    
    switch (member->type) {
        case Long:
        {
            uint64_t num = reinterpret_cast<uint64_t>(object->*(member->offset));
            return PyLong_FromLong(num);
        }

        case String:
        {
            const char* str = reinterpret_cast<char*>(object->*(member->offset));
            return Py_BuildValue("s", str);
        }

        case Object:
        {
            MetaObject* obj = reinterpret_cast<MetaObject*>(object->*(member->offset));
            return PyCharmer::Current->buildPyObject(obj);
        }

        case Void:
            Py_RETURN_NONE;
        default:
            PyErr_SetString(PyExc_AttributeError, "Bad member type");
            Py_RETURN_NONE;
    }
    Py_RETURN_NONE;
}

PyObject* PyCharmer::buildPyObject(xppr::meta::MetaObject* object) {
    if(object == nullptr) {
        Py_RETURN_NAN;
    }

    PyTypeObject* py_type = getPyType(object->m_type);
    assert(py_type != nullptr);
    MetaPyObject* py_obj = PyObject_New(MetaPyObject, py_type);
    py_obj->dict = nullptr;
    // assert(py_obj != nullptr);
    // py_obj->self = object;
    size_t n_methods = CArrSize(object->m_type->methods);
    if(n_methods != 0) {
        for(size_t i = 0; i < n_methods-1; ++i) {
            // py_obj->methods[i] = PyObject_New(MetaPyMethodObject, &MetaPyMethodObjectType);
            // py_obj->methods[i]->self = object;
            // py_obj->methods[i]->func = &object->m_type->methods[i];
            
            MetaPyMethodObject* method = PyObject_New(MetaPyMethodObject, &MetaPyMethodObjectType);
            method->self = object;
            method->func = &object->m_type->methods[i];
            xppr::log::info("Add method {}", object->m_type->methods[i].name);
            py_type->tp_setattro(reinterpret_cast<PyObject*>(py_obj), PyUnicode_FromString(object->m_type->methods[i].name), reinterpret_cast<PyObject*>(method));
        }        
    }
    Py_INCREF(py_obj);
    return reinterpret_cast<PyObject* >(py_obj);
}

PyMODINIT_FUNC 
PyInit__xpapers() {
    PyObject* module = PyModule_Create(&XPapersPyModuleDef);
    if(PyType_Ready(&MetaPyMethodObjectType) < 0) {
        return nullptr;
    }
    Py_INCREF(&MetaPyMethodObjectType);

    return module;
}

static int 
MetaSetter(PyObject* py_self, PyObject* object, void* closure) {
    const MetaMember* member = reinterpret_cast<MetaMember*>(closure);
    MetaObject* self = reinterpret_cast<MetaPyObject*>(py_self)->self;
    switch (member->type) {
        case Long:
        {
            *reinterpret_cast<uint64_t*>(&(self->*(member->offset))) = PyLong_AsLong(object);
            return 0;
        }

        case String:
        {
            PyObject* bytes = PyUnicode_AsASCIIString(object);
            if(bytes == nullptr) {
                return -1;
            }
            *reinterpret_cast<char**>(&(self->*(member->offset))) = strdup(PyBytes_AsString(bytes));
            Py_DECREF(bytes);
            return 0;
        }

        case Object:
        {
            //FIXME: Err check
            *reinterpret_cast<MetaObject**>(&(self->*(member->offset))) = reinterpret_cast<MetaPyObject*>(object)->self;
            return 0;
        }

        case Void:
        default:
            PyErr_SetString(PyExc_AttributeError, "Bad member type");
            return -1;
    }
    return -1;
}


static PyObject* 
MetaPyMethodCall(PyObject* py_self, PyObject* args, PyObject* /*kwargs*/) {
    assert(py_self->ob_type == &MetaPyMethodObjectType);
    static const size_t maxCallArgs = 6; //FIXME
    MetaPyMethodObject* self = reinterpret_cast<MetaPyMethodObject*>(py_self);
    assert(strnlen(self->func->signature, maxCallArgs + 1) <= maxCallArgs);
    ArgPack* ap = reinterpret_cast<ArgPack*>(calloc(1, sizeof(struct ArgPack) + sizeof(void*) * (maxCallArgs + 1)));
    ap->self = self->self;
    if(!PyArg_ParseTuple(args, self->func->signature, &ap->m_data[0], &ap->m_data[1], &ap->m_data[2], &ap->m_data[3], &ap->m_data[4], &ap->m_data[5])) {
        Py_RETURN_NONE;
    }
    for(size_t i = 0; self->func->signature[i]; ++i) {
        if(self->func->signature[i] == 'O') {
            ap->m_data[i] = reinterpret_cast<MetaObject*>(reinterpret_cast<MetaPyObject*>(ap->m_data[i])->self);
        }
    }
    MetaObject* retval = self->func->callback(ap);
    PyObject* ret = PyCharmer::Current->buildPyObject(retval);
    free(ap);
    return ret;
}

static PyObject* 
XPapersConnect(PyObject* /*self*/, PyObject* /*args*/) {
    return PyCharmer::Current->getMainObject();
}

void PyCharmer::addModuleMember(xppr::meta::MetaObject* object) {
    xppr::log::info("Add new module: {}", object->m_type->name);
    PyObject_SetAttrString(Connector->m_charmer->getMainObject(), object->m_type->name, buildPyObject(object));
}

PyCharmer::PyCharmer() {
    m_main_object_impl.m_type = &MainObjectType;
    
}