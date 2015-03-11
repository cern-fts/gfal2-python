#include <boost/python.hpp>
#include "LoggingHelper.h"
#include "pyGIL.h"


static PyObject* get_logger()
{
    char format[] = "s";

    PyObject* logging = PyImport_ImportModule("logging");
    PyObject* get_logger = PyObject_GetAttrString(logging, "getLogger");
    return PyObject_CallFunction(get_logger, format, "gfal2");
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
    if (!logger)
        return;

    char format[] = "s";
    PyObject_CallMethod(logger, (char*)level_method_name(level), format, message);
    Py_XDECREF(logger);
}
