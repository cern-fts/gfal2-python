/*
 * Copyright (c) CERN 2013-2015
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "GErrorWrapper.h"

using namespace PyGfal2;


PyObject *PyGfal2::GErrorPyType = NULL;


GErrorWrapper::GErrorWrapper(const std::string &msg, int error):
    _message(msg), _code(error)
{
}


GErrorWrapper::GErrorWrapper(const GError* gerr):
        _message(gerr->message), _code(gerr->code)
{
}


GErrorWrapper::~GErrorWrapper() throw()
{
}


const char *GErrorWrapper::what() const throw()
{
    return this->_message.c_str();
}


int GErrorWrapper::code() const
{
    return this->_code;
}


void GErrorWrapper::throwOnError(GError **err)
{
    if (err && *err)
    {
        std::string err_msg((*err)->message);
        int code = (*err)->code;
        g_clear_error(err);
        throw GErrorWrapper(err_msg, code);
    }
}

/*** Python Exception ***/
static PyObject* GErrorParent = PyExc_Exception;

// GError constructor
static PyObject* GError_init(PyObject* self, PyObject* args)
{
    PyObject* parent_init = NULL;
    PyObject* res         = NULL;
    PyObject* msgObj      = NULL;
    PyObject* codeObj     = NULL;
    char*     message     = NULL;
    int       code;

    // Call parent constructor
    if (!(parent_init = PyObject_GetAttrString(GErrorParent, "__init__")))
        goto exception;

    if (!(res = PyObject_CallObject(parent_init, args)))
        goto exception;

    // Setup self
    if (!PyArg_ParseTuple(args, "Osi:__init__", &self, &message, &code))
        goto exception;

    msgObj = PyString_FromString(message);
    codeObj = PyInt_FromLong(code);

    PyObject_SetAttrString(self, "message", msgObj);
    PyObject_SetAttrString(self, "code",    codeObj);

    Py_DECREF(msgObj);
    Py_DECREF(codeObj);

    return res;

exception:
    Py_XDECREF(parent_init);
    Py_XDECREF(res);
    return NULL;
}



static PyObject* GError_str(PyObject* self, PyObject* args)
{
    if (!PyArg_ParseTuple(args, "O:__str__", &self))
        return NULL;
    return PyObject_GetAttrString(self, "message");
}



static PyMethodDef GError_init_def = {
        "__init__",
        GError_init, METH_VARARGS
};



static PyMethodDef GError_str_def = {
        "__str__",
        GError_str, METH_VARARGS
};



static int add_method_to_dict(PyObject* klass, PyObject* dict, PyMethodDef* methodDef)
{
    PyObject* func   = NULL;
    PyObject* method = NULL;

    if (!(func = PyCFunction_NewEx(methodDef, NULL, NULL)))
        goto exception;

    if (!(method = PyMethod_New(func, NULL, klass)))
        goto exception;

    if (PyDict_SetItemString(dict, methodDef->ml_name, method) < 0)
        goto exception;

    Py_DECREF(method);
    Py_DECREF(func);
    return 0;

exception:
    Py_XDECREF(method);
    Py_XDECREF(func);
    return -1;
}



PyObject* PyGfal2::createGErrorExceptionType(boost::python::scope& scope)
{
    PyObject* typeObj = NULL;
    PyObject* attrs   = NULL;

    // Get name
    std::string scopeName = boost::python::extract<std::string>(scope.attr("__name__"));
    std::string qualifiedName = scopeName + ".GError";

    // Initialize class attributes
    if (!(attrs = PyDict_New()))
        goto exception;

    PyDict_SetItemString(attrs, "code", PyInt_FromLong(0));
    PyDict_SetItemString(attrs, "message", PyString_FromString(""));

    // Add methods
    if (add_method_to_dict(GErrorParent, attrs, &GError_init_def) < 0)
        goto exception;
    if (add_method_to_dict(GErrorParent, attrs, &GError_str_def) < 0)
        goto exception;

    // Create exception
    if (!(typeObj = PyErr_NewException(
            const_cast<char*>(qualifiedName.c_str()), GErrorParent, attrs))) {
        goto exception;
    }

    Py_DECREF(attrs);

    scope.attr("GError") = boost::python::borrowed(typeObj);
    return typeObj;

exception:
    Py_XDECREF(typeObj);
    Py_XDECREF(attrs);
    return NULL;
}


void PyGfal2::GError2PyError(boost::python::list& pyerrors, size_t nerrors, GError** g_errors)
{
    if (g_errors != NULL) {
        for (size_t i = 0; i < nerrors; ++i) {
            if (g_errors[i] != NULL) {
                PyObject* args = Py_BuildValue("si", g_errors[i]->message, g_errors[i]->code);
                PyObject *err = PyObject_CallObject(GErrorPyType, args);
                Py_DECREF(args);
                g_error_free(g_errors[i]);
                pyerrors.append(boost::python::handle<>(err));
            }
            else {
                pyerrors.append(boost::python::object());
            }
        }
    }
}
