// SPDX-FileCopyrightText: 2023 Gabriele Pongelli
//
// SPDX-License-Identifier: MIT

// reference https://github.com/FirefoxMetzger/mini-extension
// reference https://realpython.com/build-python-c-extension-module/
// reference https://www.pythonsheets.com/notes/python-c-extensions.html
// ref       https://pysheeet.readthedocs.io/en/latest/notes/python-c-extensions.html

// ref https://github.com/astropy/extension-helpers/blob/main/extension_helpers/src/compiler.c
// ref https://github.com/felipec/libmtag-python/blob/master/libmtagmodule.c
// ref https://stackoverflow.com/questions/58159184/implementing-unit-tests-for-a-python-c-extension


#ifndef PY_SSIZE_T_CLEAN
#define PY_SSIZE_T_CLEAN
#endif

#include <Python.h>
#include <structmember.h>

#include <stdio.h>
#include <stdlib.h>

#include "code128.h"

// https://ishantheperson.github.io/posts/python-c-ext/
#if PY_MAJOR_VERSION < 3
#error "PyCode128 requires Python 3"
#include "stopcompilation"  // non-existent file to stop compilation
#endif

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
        do barcode and save encoded_data and length
    def encode_raw():
        do barcode and save encoded_data and length
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
    PyObject_HEAD

    PyObject *input_data;
    PyObject *encoded_data;
    PyObject *length;
} PyCode128Object;



/* methods implementation */
static PyObject* estimate_len(PyCode128Object *self, PyObject *Py_UNUSED(ignored))
{
    const char *data;
    size_t barcode_len = 0;

    /* check for input data */
    if (self->input_data == NULL) {
        PyErr_SetString(PyExc_AttributeError, "Input data is missing.");
        return NULL;
    }

    /* Parse argument, expected a const char *
     *  ref  https://docs.python.org/3/c-api/arg.html
     *  PyArg_ParseTuple converts PyObject to C type
     */
    if (!PyArg_ParseTuple(self->input_data, "s", &data)) {
        // PyArg_ParseTuple evaluate to false on failure
        return NULL;
    }

    barcode_len = code128_estimate_len(data);

    return PyLong_FromSsize_t(barcode_len);
}


static PyObject* encode_gs1(PyCode128Object *self, PyObject *Py_UNUSED(ignored))
{
/*  size_t ADDCALL code128_encode_gs1(const char *s, char *out, size_t maxlength) */
    const char *data;
    char *barcode_data;  // out value
    size_t max_length = 0;
    size_t barcode_len = 0; // out value
    PyObject *pyobj_encoded = NULL, *pyobj_length = NULL, *tmp_enc = NULL, *tmp_len = NULL;

    /* check for input data */
    if (self->input_data == NULL) {
        PyErr_SetString(PyExc_AttributeError, "Input data is missing.");
        return NULL;
    }

    /* Parse argument, expected a const char *
     *  ref  https://docs.python.org/3/c-api/arg.html
     *  PyArg_ParseTuple converts PyObject to C type
     */
    if(!PyArg_ParseTuple(self->input_data, "s", &data)) {
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

    // Py_BuildValue creates PyObject
    pyobj_encoded = Py_BuildValue("s", barcode_data);  // check if bytes object is better
    free(barcode_data);
    if (pyobj_encoded != NULL) {
        tmp_enc = self->encoded_data;
        Py_INCREF(pyobj_encoded);
        self->encoded_data = pyobj_encoded;
        Py_XDECREF(tmp_enc);
    } else {
        /* PyBuild_value:
           Returns the value or NULL in the case of an error; an exception will be raised if NULL is returned */
    }

    pyobj_length = Py_BuildValue("i", barcode_len);
    if (pyobj_length != NULL) {
        tmp_len = self->length;
        Py_INCREF(pyobj_length);
        self->length = pyobj_length;
        Py_XDECREF(tmp_len);
    } else {
        /* PyBuild_value:
           Returns the value or NULL in the case of an error; an exception will be raised if NULL is returned */
    }

    Py_RETURN_NONE;
}



static PyObject* encode_raw(PyCode128Object *self, PyObject *Py_UNUSED(ignored))
{
/* size_t ADDCALL code128_encode_raw(const char *s, char *out, size_t maxlength) */
    const char *data;
    char *barcode_data;  // out value
    size_t max_length = 0;
    size_t barcode_len = 0; // out value
    PyObject *pyobj_encoded = NULL, *pyobj_length = NULL, *tmp_enc = NULL, *tmp_len = NULL;

    /* check for input data */
    if (self->input_data == NULL) {
        PyErr_SetString(PyExc_AttributeError, "Input data is missing.");
        return NULL;
    }

    /* Parse argument, expected a const char *
     *  ref  https://docs.python.org/3/c-api/arg.html
     *  PyArg_ParseTuple converts PyObject to C type
     */
    if(!PyArg_ParseTuple(self->input_data, "s", &data)) {
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

    // Py_BuildValue creates PyObject
    pyobj_encoded = Py_BuildValue("s", barcode_data);  // check if bytes object is better
    free(barcode_data);
    if (pyobj_encoded != NULL) {
        tmp_enc = self->encoded_data;
        Py_INCREF(pyobj_encoded);
        self->encoded_data = pyobj_encoded;
        Py_XDECREF(tmp_enc);
    } else {
        /* PyBuild_value:
           Returns the value or NULL in the case of an error; an exception will be raised if NULL is returned */
    }

    pyobj_length = Py_BuildValue("i", barcode_len);
    if (pyobj_length != NULL) {
        tmp_len = self->length;
        Py_INCREF(pyobj_length);
        self->length = pyobj_length;
        Py_XDECREF(tmp_len);
    } else {
        /* PyBuild_value:
           Returns the value or NULL in the case of an error; an exception will be raised if NULL is returned */
    }

    Py_RETURN_NONE;
}



PyDoc_STRVAR(estimate_len_doc,  "Returns label's estimated length.");
PyDoc_STRVAR(encode_gs1_doc,    "Encode the GS1 string.\nReturns the length of barcode data in bytes");
PyDoc_STRVAR(encode_raw_doc,    "Encode raw string.\nReturns the length of barcode data in bytes");

/* methods definition */
// https://docs.python.org/3/c-api/structures.html#c.PyMethodDef
static PyMethodDef PyCode128_methods[] = {
    /*  ml_name,                ml_meth,              ml_flags,         ml_doc           */
    {"estimate_len",    (PyCFunction)estimate_len,   METH_NOARGS,   estimate_len_doc},
    {"encode_gs1",      (PyCFunction)encode_gs1,     METH_NOARGS,   encode_gs1_doc},
    {"encode_raw",      (PyCFunction)encode_raw,     METH_NOARGS,   encode_raw_doc},
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
                                    "U", keywords,  // unicode string is not optional
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


PyDoc_STRVAR(input_data_doc, "Input string to be converted in Code128.");
static PyGetSetDef PyCode128_getsetters[] = {
    /*    name,                   get,                           set,                               doc,        closure  */
    {"input_data",      (getter)PyCode128_get_input_data,   (setter)PyCode128_set_input_data,   input_data_doc },
    {"encoded_data",    (getter)PyCode128_get_encoded_data, NULL},  // read-only
    {"length",          (getter)PyCode128_get_length,       NULL},  // read-only
};


static PyObject *
PyCode128_str(PyObject * obj) {
    return PyUnicode_FromFormat("PyCode128 instance");
}


static PyObject *
PyCode128_richcompare(PyObject *obj1, PyObject *obj2, int op) {
    PyObject *result;
    int c = 0;
    PyObject *input_data1, *input_data2;

    // check that input pyobject is PyCode128Object is missing

    input_data1 = ((PyCode128Object *) obj1)->input_data;
    input_data2 = ((PyCode128Object *) obj2)->input_data;

    switch (op) {
        case Py_LT: c = input_data1 <  input_data2; break;
        case Py_LE: c = input_data1 <= input_data2; break;
        case Py_EQ: c = input_data1 == input_data2; break;
        case Py_NE: c = input_data1 != input_data2; break;
        case Py_GT: c = input_data1 >  input_data2; break;
        case Py_GE: c = input_data1 >= input_data2; break;
    }
    result = c ? Py_True : Py_False;
    Py_INCREF(result);
    return result;
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
    .tp_str = PyCode128_str,
    .tp_richcompare = PyCode128_richcompare
};


/********************************
 *   Python module definition   *
 ********************************/

PyDoc_STRVAR(module_doc, "Extension for code128 library");

/* module definition */
static struct PyModuleDef pycode128_module = {
    PyModuleDef_HEAD_INIT,
    "pycode128.pycode128",  /* m_name */
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
