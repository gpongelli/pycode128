// reference https://github.com/FirefoxMetzger/mini-extension
// reference https://realpython.com/build-python-c-extension-module/
// reference https://www.pythonsheets.com/notes/python-c-extensions.html

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <structmember.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>


#include "code128.h"


/*

ADDAPI size_t ADDCALL code128_estimate_len(const char *s);
ADDAPI size_t ADDCALL code128_encode_gs1(const char *s, char *out, size_t maxlength);
ADDAPI size_t ADDCALL code128_encode_raw(const char *s, char *out, size_t maxlength);
*/

/*
-- form pycode.pycode import PyCode128
-- c = PyCode('abcdef')
-- c.encode_raw()
-- out = c.encoded_data()

class PyCode128:

    def __init__(input_data):
        self.input_data = input_data

    def encode_gs1():
        get char*
    def encode_raw():
        get char*
    def estimate_len():
        get estimated len

    @prop get/set input_data()
        get input data
    @prop get encoded_data():
        get char*
    @prop get length():
        get estimated len
*/

typedef struct {
    PyObject_HEAD,
    PyObject *input_data;
    PyObject *encoded_data;
    PyObject *length;
} PyCode128Object;



/* methods implementation */
static PyObject* code128_estimate_len(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    const char *data;
    size_t barcode_len = 0;

    /* Parse argument, expected a const char *
     *  ref  https://docs.python.org/3/c-api/arg.html
     */
    /* if(!PyArg_ParseTuple(args, "s", &data)) {
        // PyArg_ParseTuple evaluate to false on failure
        return NULL;
    }*/
    if (self->input_data == NULL) {
        PyErr_SetString(PyExc_AttributeError, "Input data is missing.");
        return NULL;
    }
//////////////////////////

    barcode_len = code128_estimate_len(data);

    return PyLong_FromLong(barcode_len);
}


static PyObject* code128_encode_gs1(PyObject *self, PyObject *Py_UNUSED(ignored))
{
/*  size_t ADDCALL code128_encode_gs1(const char *s, char *out, size_t maxlength) */
    const char *data;
    char *barcode_data;  // out value
    size_t max_length = 0;
    size_t barcode_len = 0; // out value

    /* Parse argument, expected a const char *
     *  ref  https://docs.python.org/3/c-api/arg.html
     */
    if(!PyArg_ParseTuple(args, "s", &data)) {
        // PyArg_ParseTuple evaluate to false on failure
        return NULL;
    }

    // get barcode length and allocate output char *
    max_length = code128_estimate_len(data);
    barcode_data = (char *) malloc(max_length * 2);
    if (barcode_data == NULL) {
        return NULL;
    }

    barcode_len = code128_encode_gs1(data, &barcode_data[0], max_length);

    return PyLong_FromLong(barcode_len);
}



static PyObject* code128_encode_raw(PyObject *self, PyObject *Py_UNUSED(ignored))
{
/* size_t ADDCALL code128_encode_raw(const char *s, char *out, size_t maxlength) */
    const char *data;
    char *barcode_data;  // out value
    size_t max_length = 0;
    size_t barcode_len = 0; // out value

    /* Parse argument, expected a const char *
     *  ref  https://docs.python.org/3/c-api/arg.html
     */
    if(!PyArg_ParseTuple(args, "s", &data)) {
        // PyArg_ParseTuple evaluate to false on failure
        return NULL;
    }

    // get barcode length and allocate output char *
    max_length = code128_estimate_len(data);
    barcode_data = (char *) malloc(max_length * 2);
    if (barcode_data == NULL) {
        return NULL;
    }

    barcode_len = code128_encode_raw(data, &barcode_data[0], max_length);

    return PyLong_FromLong(barcode_len);
}



PyDoc_STRVAR(code128_estimate_len_doc,  "Returns label's estimated length.");
PyDoc_STRVAR(code128_encode_gs1_doc,    "Encode the GS1 string.\nReturns the length of barcode data in bytes");
PyDoc_STRVAR(code128_encode_raw_doc,    "Encode raw string.\nReturns the length of barcode data in bytes");

/* methods definition */
// https://docs.python.org/3/c-api/structures.html#c.PyMethodDef
static PyMethodDef PyCode128_methods[] = {
    /*  ml_name,                                 ml_meth,              ml_flags,         ml_doc           */
    {"code128_estimate_len",    (PyCFunction)code128_estimate_len,   METH_NOARGS,   code128_estimate_len_doc},
    {"code128_encode_gs1",      (PyCFunction)code128_encode_gs1,     METH_NOARGS,   code128_encode_gs1_doc},
    {"code128_encode_raw_doc",  (PyCFunction)code128_encode_raw,     METH_NOARGS,   code128_encode_raw_doc},
    {NULL}  /* Sentinel */
};


/********************
 *   Type Methods   *
 ********************/

static PyObject *
PyCode128_new(PyTypeObject *type, PyObject *args, PyObject *kw)
{
    // https://docs.python.org/3/extending/newtypes_tutorial.html
    PyCode128Object *self = NULL;
    self = (PyCode128Object *) type->tp_alloc(type, 0);

    if (self != NULL) {
        /* allocate attribute */
        self->input_data = PyUnicode_FromString("");
        if (self->input_data == NULL) {
            Py_XDECREF(self->input_data);
            Py_XDECREF(self);
            // self is probably not null here, so force the return value
            return NULL;
        }
    }
    return (PyObject *) self;
}


static int
PyCode128_init(PyCode128Object *self, PyObject *args, PyObject *kw)
{
    static char *keywords[] = {"input_data", NULL};
    PyObject *input_data = NULL, *tmp = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kw,
                                    "|U", keywords,
                                    &input_data))
    {
        // https://docs.python.org/3/extending/newtypes_tutorial.html
        /* Initializers always accept positional and keyword arguments,
           and they should return either 0 on success or -1 on error. */
        return -1;
    }

    if (input_data && PyUnicode_Check(input_data)) {
        tmp = self->input_data;
        Py_INCREF(input_data);
        self->input_data = input_data;
        Py_XDECREF(tmp);
    }

    return 0;
}


static void
PyCode128_dealloc(PyCode128Object *self)
{
    Py_XDECREF(self->input_data);
    Py_XDECREF(self->encoded_data);
    Py_XDECREF(self->length);
    Py_TYPE(self)->tp_free((PyObject *) self);
}



// https://docs.python.org/3/c-api/structures.html#c.PyMemberDef
PyDoc_STRVAR(input_data_doc, "Input string to be converted in Code128.");
static PyMemberDef PyCode128_members[] = {
     /* name        type                  offset                      flags          doc  */
    /* {"input_data", T_STRING, offsetof(PyCode128Object, input_data),  READONLY,   input_data_doc}, */
    {NULL}  /* Sentinel */
};


/***************************
 *   Getters and setters   *
 ***************************/

static PyObject *
PyCode128_get_input_data(PyCode128Object *self, void *closure)
{
    Py_INCREF(self->input_data);
    return self->input_data;
}

static int
PyCode128_set_input_data(PyCode128Object *self, PyObject *value, void *closure)
{
    int rc = -1;
    PyObject *tmp;

    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the input_data attribute.");
    } else {
        if (!PyUnicode_Check(value)) {
            PyErr_SetString(PyExc_TypeError, "value should be unicode");
        } else {
            rc = 0; // return value
            tmp = self->input_data;
            Py_INCREF(value);
            self->input_data = value;
            Py_DECREF(tmp);
        }
    }

    return rc;
}

static PyObject *
PyCode128_get_encoded_data(PyCode128Object *self, void *closure)
{
    Py_INCREF(self->encoded_data);
    return self->encoded_data;
}


static PyObject *
PyCode128_get_length(PyCode128Object *self, void *closure)
{
    Py_INCREF(self->length);
    return self->length;
}


static PyGetSetDef PyCode128_getsetters[] = {
    /*    name,                   get,                           set,                               doc,        closure  */
    {"input_data",      (getter)PyCode128_get_input_data,   (setter)PyCode128_set_input_data,   input_data_doc },
    {"encoded_data",    (getter)PyCode128_get_encoded_data, NULL},  // read-only
    {"length",          (getter)PyCode128_get_length,       NULL},  // read-only
};


static PyObject *
PyCode128_str(PyCode128Object * obj) {
    return PyUnicode_FromFormat("PyCode128 instance for %s ", obj->input_data);
}


PyDoc_STRVAR(pycode128_type_doc, "PyCode128 object");
static PyTypeObject PyCode128Type = {
    PyVarObject_HEAD_INIT(NULL, 0)

    .tp_name = "pycode128.PyCode128",
    .tp_doc = pycode128_type_doc,
    .tp_basicsize = sizeof(PyCode128Object),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyCode128_new,
    .tp_init = (initproc) PyCode128_init,
    .tp_dealloc = (destructor) PyCode128_dealloc,
    .tp_getset = PyCode128_getsetters,
    .tp_members = PyCode128_members,
    .tp_methods = PyCode128_methods,
    .tp_str = PyCode128_str
};


/********************************
 *   Python module definition   *
 ********************************/

PyDoc_STRVAR(module_doc, "Extension for code128 library");

/* module definition */
static struct PyModuleDef pycode128_module = {
    PyModuleDef_HEAD_INIT,
    "pycode128",            /* m_name */   // "pycode128.pycode128" o solo "pycode128"
    module_doc,             /* m_doc */
    -1,                     /* m_size */
    NULL,                   /* m_methods */  // no module function, but a class
    NULL,                   /* m_reload */
    NULL,                   /* m_traverse */
    NULL,                   /* m_clear */
    NULL,                   /* m_free */
};


static PyObject* module_init(void) {
    PyObject *module;

    if (PyType_Ready(&PyCode128Type) < 0)
    {
        return NULL;
    }

    if ((module = PyModule_Create(&pycode128_module)) == NULL)
    {
        return NULL;
    }

    /* Add macros to module */
    PyModule_AddIntMacro(module, CODE128_FNC1);
    PyModule_AddIntMacro(module, CODE128_FNC2);
    PyModule_AddIntMacro(module, CODE128_FNC3);
    PyModule_AddIntMacro(module, CODE128_FNC4);

    Py_XINCREF(&PyCode128Type);

    if (PyModule_AddObject(module, "PyCode128", (PyObject *) &PyCode128Type) < 0) {
        Py_DECREF(&PyCode128Type);
        Py_DECREF(&module);
        return NULL;
    }

    return module;
}

/* module init */
PyMODINIT_FUNC PyInit_pycode128(void)
{
    return module_init();
}
