/*
* Copyright @ CERN, 2014-2015
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

#include <boost/python.hpp>
#include "LoggingHelper.h"
#include "pyGIL.h"


static PyObject* get_logger(const char *name = "gfal2")
{
    static char format[] = "s";

    PyObject* logging = PyImport_ImportModule("logging");
    PyObject* getLogger = NULL;
    if (logging != NULL) {
        getLogger = PyObject_GetAttrString(logging, "getLogger");
    } else return NULL;
    if (getLogger != NULL) {
         return PyObject_CallFunction(getLogger, format, name);
    } else  return NULL;
}


static const char *level_method_name(GLogLevelFlags level)
{
    switch (level) {
        case G_LOG_LEVEL_ERROR:
            return "error";
        case G_LOG_LEVEL_CRITICAL:
            return "critical";
        case G_LOG_LEVEL_WARNING:
            return "warning";
        case G_LOG_LEVEL_MESSAGE:
        case G_LOG_LEVEL_INFO:
            return "info";
        case G_LOG_LEVEL_DEBUG:
        default:
            return "debug";
    }
}


void PyGfal2::logging_helper(const gchar *log_domain, GLogLevelFlags level,
        const gchar *message, gpointer user_data)
{
    ScopedGILLocker lock;

    PyObject* logger = get_logger();
    if (!logger) {
        return;
    }

    char format[] = "s";
    PyObject_CallMethod(logger, (char*)level_method_name(level), format, message);
    Py_XDECREF(logger);
}


PyGfal2::NullHandler::NullHandler(): level(50) {
}


void PyGfal2::NullHandler::createLock(void)
{}


void PyGfal2::NullHandler::acquire(void)
{}


void PyGfal2::NullHandler::release(void)
{}


void PyGfal2::NullHandler::setLevel(int)
{}


void PyGfal2::NullHandler::setFormatter(boost::python::object)
{}


void PyGfal2::NullHandler::addFilter(boost::python::object)
{}


void PyGfal2::NullHandler::removeFilter(boost::python::object)
{}


void PyGfal2::NullHandler::filter(boost::python::object)
{}


void PyGfal2::NullHandler::flush()
{}


void PyGfal2::NullHandler::close()
{}


void PyGfal2::NullHandler::handle(boost::python::object)
{}


void PyGfal2::NullHandler::handleError(boost::python::object)
{}


void PyGfal2::NullHandler::format(boost::python::object)
{}


void PyGfal2::NullHandler::emit(boost::python::object)
{
}


void PyGfal2::logging_register_handler(const char *name, boost::python::object handler)
{
    PyObject *cLogger = get_logger(name);
    if (!cLogger) {
        return;
    }

    boost::python::object logger(boost::python::handle<>(boost::python::borrowed(cLogger)));
    logger.attr("addHandler")(handler);
}
