/*
* Copyright @ Members of the EMI Collaboration, 2013.
* See www.eu-emi.eu for details on the copyright holders.
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
#include "gfal_boost_include.hpp"

/*** GErrorWrapper ***/

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



static int add_method_to_dict(PyObject* dict, PyMethodDef* methodDef)
{
    PyObject* func   = NULL;
    PyObject* method = NULL;

    if (!(func = PyCFunction_NewEx(methodDef, NULL, NULL)))
        goto exception;

    if (!(method = PyMethod_New(func, NULL, NULL)))
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



PyObject* createGErrorException()
{
    namespace bp = boost::python;

    PyObject* typeObj = NULL;
    PyObject* attrs   = NULL;

    // Get name
    std::string scopeName = bp::extract<std::string>(bp::scope().attr("__name__"));
    std::string qualifiedName = scopeName + ".GError";

    // Initialize class attributes
    if (!(attrs = PyDict_New()))
        goto exception;

    PyDict_SetItemString(attrs, "code", PyInt_FromLong(0));
    PyDict_SetItemString(attrs, "message", PyString_FromString(""));

    if (add_method_to_dict(attrs, &GError_init_def) < 0)
        goto exception;
    if (add_method_to_dict(attrs, &GError_str_def) < 0)
        goto exception;

    // Create exception
    if (!(typeObj = PyErr_NewException(
            const_cast<char*>(qualifiedName.c_str()), GErrorParent, attrs)))
        goto exception;

    Py_DECREF(attrs);
    bp::scope().attr("GError") = bp::handle<>(bp::borrowed(typeObj));
    return typeObj;

exception:
    Py_XDECREF(typeObj);
    Py_XDECREF(attrs);
    return NULL;
}
