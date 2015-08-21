/*
 * Copyright (c) CERN 2013-2015
 *
 * Copyright (c) Members of the EMI Collaboration. 2010-2013
 *  See  http://www.eu-emi.eu/partners for details on the copyright
 *  holders.
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

#ifndef GFALT_PARAMS_H
#define GFALT_PARAMS_H

#include <gfal_api.h>
#include <sstream>
#include <transfer/gfal_transfer.h>

#include "GErrorWrapper.h"

namespace PyGfal2 {

// Convenience wrapper for gfal_event_t
class GfaltEvent {
public:
    int         side;
    gint64      timestamp;
    std::string stage;
    std::string domain;
    std::string description;

    GfaltEvent(): side(0), timestamp(0) {
    }

    GfaltEvent(gfalt_event_t e): side(e->side), timestamp(e->timestamp),
            description(e->description)
    {
        stage  = g_quark_to_string(e->stage);
        domain = g_quark_to_string(e->domain);
    }

    std::string __str__(void) const {
        std::ostringstream stream;

        stream << '[' << timestamp << "] ";

        switch (side) {
        case GFAL_EVENT_SOURCE:
            stream << "SOURCE ";
            break;
        case GFAL_EVENT_DESTINATION:
            stream << "DEST   ";
            break;
        default:
            stream << "BOTH   ";
            break;
        }

        stream << domain << '\t'
               << stage << '\t'
               << description;

        return stream.str();
    }

};

// Callback objects
// Only one, since the user data is shared between event and monitoring callbacks
struct CallbackObjs {
    boost::python::object event_callback;
    boost::python::object monitor_callback;
};

// Wrapper for gfalt_params_t
class GfaltParams
{
private:
    gfalt_params_t params;
    CallbackObjs callback_objs;

public:
    GfaltParams();
    GfaltParams(const GfaltParams & s);

    GfaltParams copy();

    virtual ~GfaltParams();

    void                 set_timeout(guint64 timeout);
    guint64              get_timeout();
    void                 set_nbstream(guint nbstream);
    guint                get_nbstream();
    void                 set_checksum_check(bool checksum_check);
    bool                 get_checksum_check();
    void                 set_src_spacetoken(const std::string & token);
    std::string          get_src_spacetoken();
    void                 set_dst_spacetoken(const std::string & token);
    std::string          get_dst_spacetoken();
    void                 set_user_defined_checksum(const std::string & chk_type,
                            const std::string & checksum);
    boost::python::tuple get_user_defined_checksum();
    void                 set_overwrite(bool overwrite);
    bool                 get_overwrite(void);
    void                 set_create_parent(bool create_parent);
    bool                 get_create_parent(void);
    void                 set_tcp_buffersize(uint64_t buffersize);
    uint64_t             get_tcp_buffersize(void);
    bool                 get_strict_copy(void);
    void                 set_strict_copy(bool val);

    // Callbacks
    void      set_event_callback(PyObject* callable);
    PyObject* get_event_callback(void);
    void      set_monitor_callback(PyObject* callable);
    PyObject* get_monitor_callback(void);

    friend class Gfal2Context;
};

}

#endif // GFALT_PARAMS_H
