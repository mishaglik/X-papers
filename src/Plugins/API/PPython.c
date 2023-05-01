#include <Python.h>
#include "stddef.h"

struct ArgPack {
    const char* m_signature;
    struct MetaClass* self;
    void* m_data[];
};

struct MetaClass;
struct MetaFunction {
    const char* m_name;
    const char* m_signature;
    struct MetaClass* (*m_callback)(struct ArgPack* ap);
};

struct MetaMember {
    const char* m_name;
    char type;
    void* data;
};

struct MetaClass {
    const char* m_name;
    struct MetaFunction* m_methods; // NULL terminated
    struct MetaMember*   m_members; // NULL terminated
};

struct py_meta_class {
    PyObject_HEAD
    struct MetaClass* mt;
    PyObject* dict;
};

struct py_meta_method {
    PyObject_HEAD
    PyObject* name;
    struct MetaFunction* mf;
    struct MetaClass* mt;
};

static PyObject* build_py_class(struct MetaClass* meta_class);

static struct MetaClass* xmain = NULL;
static struct PyObject* main_obj = NULL;
static PyObject* func_connect(PyObject *self, PyObject *args) {
    PyObject* obj = build_py_class(xmain);
    Py_INCREF(obj);
    main_obj = obj;
    fprintf(stderr, "class builded\n");
    return Py_BuildValue("O", obj);
}

// Список функций модуля
static PyMethodDef xpapers_methods[] = {
    {"connect", func_connect, METH_NOARGS, "connect"},
    {NULL, NULL, 0, NULL}
};

// Описание модуля
static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT, "xpapers", "Xpapers module", -1, xpapers_methods
};

static void
py_meta_class_dealloc(struct py_meta_class* self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
py_meta_class_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    struct py_meta_class *self;

    self = (struct py_meta_class *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->dict = NULL;
    }

    return (PyObject *)self;
}

// Инициализация структуры, заполняем её переданными значениями
static int
py_meta_class_init(struct py_meta_class *self, PyObject *args, PyObject *kwds) {
    // static char *kwlist[] = {"val1", "val2", "val3", NULL};

    // if (! PyArg_ParseTupleAndKeywords(args, kwds, "|idb", kwlist, &self->val1, &self->val2, &self->val3))
    //     return -1;

    return 0;
}

PyTypeObject py_meta_class_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "xpapers.py_meta_class",   /* tp_name */
    sizeof(struct py_meta_class),         /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor) py_meta_class_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "py_meta_class objects",       /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    offsetof(struct py_meta_class, dict),                         /* tp_dictoffset */
    (initproc) py_meta_class_init, /* tp_init */
    0,                         /* tp_alloc */
    py_meta_class_new,         /* tp_new */
};

static void
py_meta_method_dealloc(struct py_meta_method* self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
py_meta_method_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    struct py_meta_method *self;

    self = (struct py_meta_method *)type->tp_alloc(type, 0);
    if (self != NULL) {
    }

    return (PyObject *)self;
}

// Инициализация структуры, заполняем её переданными значениями
static int
py_meta_method_init(struct py_meta_class *self, PyObject *args, PyObject *kwds) {
    // static char *kwlist[] = {"val1", "val2", "val3", NULL};

    // if (! PyArg_ParseTupleAndKeywords(args, kwds, "|idb", kwlist, &self->val1, &self->val2, &self->val3))
    //     return -1;

    return 0;
}

static PyObject* py_meta_method_call(PyObject *self, PyObject *args, PyObject *kwargs);

PyTypeObject py_meta_method_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "xpapers.py_meta_method",         /* tp_name */
    sizeof(struct py_meta_method),         /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor) py_meta_method_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    py_meta_method_call,       /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "test_st_t objects",       /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc) py_meta_method_init, /* tp_init */
    0,                         /* tp_alloc */
    0,             /* tp_new */
};

PyMODINIT_FUNC
PyInit__xpapers(void) {
    
    PyObject* mod = PyModule_Create(&module);

    if (PyType_Ready(&py_meta_class_Type) < 0)
        return NULL;
    PyModule_AddType(mod, &py_meta_class_Type);
    PyModule_AddType(mod, &py_meta_method_Type);
    assert(py_meta_class_Type.tp_setattro != 0);
    return mod;
}




PyObject* build_py_meth(struct MetaFunction* meta_method, struct MetaClass* meta_class) {
    struct py_meta_method* obj = PyObject_New(struct py_meta_method, &py_meta_method_Type);
    obj->mt = meta_class;
    obj->mf = meta_method;
    return Py_BuildValue("O", obj);
}

PyObject* build_py_memb(struct MetaMember* meta_member) {
    // struct py_meta_method* obj = PyObject_New(struct py_meta_method, &py_meta_method_Type);
    // obj->mt = meta_class;
    // obj->mf = meta_method;
    // return (PyObject* )obj;
    switch (meta_member->type) {
        case 'L':
            return Py_BuildValue("L", meta_member->data);
        case 's':
            return Py_BuildValue("s", meta_member->data);
        case 'z':
            return Py_BuildValue("z", meta_member->data);
        case 'O':
            return Py_BuildValue("O", build_py_class(meta_member->data));
        default: 
            return NULL;
    }
}

PyObject* build_py_class(struct MetaClass* meta_class) {
    struct py_meta_class* obj = py_meta_class_new(&py_meta_class_Type, 0, 0);
    obj->mt = meta_class;

    if(meta_class->m_methods) {
        for(struct MetaFunction* meth = meta_class->m_methods; meth->m_name != NULL; meth++) {
            assert(meth);
            fprintf(stderr, "Init meth %s\n", meth->m_name);
            
            obj->ob_base.ob_type->tp_setattro((PyObject* )obj, PyUnicode_FromString(meth->m_name), build_py_meth(meth, meta_class));
            fprintf(stderr, "Inited meth %s\n", meth->m_name);
        }
    }

    if(meta_class->m_members) {
        for(struct MetaMember* memb = meta_class->m_members; memb->m_name != NULL; memb++) {
            PyObject_SetAttrString((PyObject* )obj, memb->m_name, build_py_memb(memb));
        }
    }
    fprintf(stderr, "Object inited %s\n", meta_class->m_name);
    return (PyObject* )obj;
}

static PyObject* py_meta_method_call(PyObject *self, PyObject *args, PyObject *kwargs)
{   

    const size_t maxArgs = 6;
    assert(self->ob_type == &py_meta_method_Type);
    struct py_meta_method* selfm = (struct py_meta_method*)self;
    assert(strlen(selfm->mf->m_signature) <= maxArgs);
    struct ArgPack* ap = calloc(1, sizeof(struct ArgPack) + sizeof(void*) * (maxArgs + 1));
    ap->self = selfm->mt;
    fprintf(stderr, "Meta call with signature: '%s'\n", selfm->mf->m_signature);
    if(!PyArg_ParseTuple(args, selfm->mf->m_signature, &ap->m_data[0], &ap->m_data[1], &ap->m_data[2], &ap->m_data[3], &ap->m_data[4], &ap->m_data[5])) {
        Py_RETURN_NONE;
    }
    for(size_t i = 0; selfm->mf->m_signature[i]; ++i) {
        if(selfm->mf->m_signature[i] == 'O') {
            ap->m_data[i] += offsetof(struct py_meta_class, mt);
        }
    }
    struct MetaClass* mc = NULL;
    if((mc = selfm->mf->m_callback(ap))) {
        return build_py_class(mc);
    }
    Py_RETURN_NONE;
}

void init_python_module(struct MetaClass* main);

void init_python_module(struct MetaClass* main) {
    xmain = main;
    wchar_t *program = Py_DecodeLocale("pypapers", NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    Py_SetProgramName(program);  /* optional but recommended */
    PyImport_AppendInittab("xpapers", &PyInit__xpapers);
    Py_Initialize();
    PyRun_SimpleString(
        "from time import time,ctime\n"
        "print('Today is', ctime(time()))\n"
        "import xpapers\n"
        "xpap = xpapers.connect()\n"
        "print('Connected')\n"
        "print(xpap.__dir__())\n"
        "xpap.print(2)\n"
        "xpap.loadModule('bg')\n"
        "print(xpap.bg.__dir__())\n"
        "xpap.bg.setImage('/home/gms/I1.jpg')\n"
        );
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    PyMem_RawFree(program);
    fprintf(stderr, "Pyscript finished\n");
    return;
}

void reg_class(struct MetaClass* meta) {
    PyObject_SetAttrString(main_obj, meta->m_name, build_py_class(meta));
}


