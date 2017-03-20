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

#ifndef LOGGINGHELPER_H
#define LOGGINGHELPER_H

#include <gfal_api.h>

namespace PyGfal2 {

/**
 * Makes the use of the Python logging facilities easier
 */
void logging_helper(const gchar *log_domain, GLogLevelFlags log_level,
        const gchar *message, gpointer user_data);

/** Default handler for gfal2
 * Avoids "no handlers could be found for logger gfal2"
 * I can't figure out how to inherit from logging.Handler, so I just added all methods here
 */
class NullHandler {
public:
    int level;
    boost::python::list filters;

    NullHandler();

    void createLock(void);
    void acquire(void);
    void release(void);
    void setLevel(int lvl);
    void setFormatter(boost::python::object form);
    void addFilter(boost::python::object filt);
    void removeFilter(boost::python::object filt);
    void filter(boost::python::object record);
    void flush();
    void close();
    void handle(boost::python::object record);
    void handleError(boost::python::object record);
    void format(boost::python::object record);
    void emit(boost::python::object record);
};

/** Register a handler for the given logger name
 */
void logging_register_handler(const char *name, boost::python::object handler);

};

#endif // LOGGINGHELPER_H
