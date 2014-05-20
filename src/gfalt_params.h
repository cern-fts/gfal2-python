/*
* Copyright @ Members of the EMI Collaboration, 2010.
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

#ifndef GFALT_PARAMS_H
#define GFALT_PARAMS_H

#include <gfal_api.h>
#include <sstream>
#include <transfer/gfal_transfer.h>

#include "gfalcpp.h"

// Convenience wrapper for gfal_event_t
class Gfalt_event {
public:
    int         side;
    gint64      timestamp;
    std::string stage;
    std::string domain;
    std::string description;

    Gfalt_event(): side(0), timestamp(0) {
    }

    Gfalt_event(gfalt_event_t e): side(e->side), timestamp(e->timestamp),
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

// Event callback
void event_callback_wrapper(const gfalt_event_t e, gpointer user_data);

// Monitor callback
void monitor_callback_wrapper(gfalt_transfer_status_t h, const char* src, const char* dst, gpointer user_data);

// Calback objects
// Only one, since the user data is shared between event and monitoring callbacks
struct CallbackObjs {
    boost::python::object event_callback;
    boost::python::object monitor_callback;
};

// Wrapper for gfalt_params_t
class Gfalt_params
{
private:
    gfalt_params_t params;
    CallbackObjs callback_objs;

public:
    Gfalt_params(){
        GError * tmp_err=NULL;
        params = gfalt_params_handle_new(&tmp_err);
        check_GError(&tmp_err);
    }

    Gfalt_params(const Gfalt_params & s){
        GError * tmp_err=NULL;
        params = gfalt_params_handle_copy(s.params,&tmp_err );
        check_GError(&tmp_err);
    }

    Gfalt_params copy(){
        return Gfalt_params(*this);
    }

    virtual ~Gfalt_params(){
        gfalt_params_handle_delete(params,NULL);
    }

    void set_timeout(guint64 timeout){
        GError * tmp_err=NULL;
        gfalt_set_timeout(params, timeout, &tmp_err);
        check_GError(&tmp_err);
    }

    guint64 get_timeout(){
        GError * tmp_err=NULL;
        guint64 res = gfalt_get_timeout(params, &tmp_err);
        check_GError(&tmp_err);
        return res;
    }

    void set_nbstream(guint nbstream){
        GError * tmp_err=NULL;
        gfalt_set_nbstreams(params, nbstream, &tmp_err);
        check_GError(&tmp_err);
    }

    guint get_nbstream(){
        GError * tmp_err=NULL;
        guint res = gfalt_get_nbstreams(params, &tmp_err);
        check_GError(&tmp_err);
        return res;
    }

    void set_checksum_check(bool checksum_check){
        GError * tmp_err=NULL;
        gfalt_set_checksum_check(params, checksum_check, &tmp_err);
        check_GError(&tmp_err);
    }


    bool get_checksum_check(){
        GError * tmp_err=NULL;
        bool res = gfalt_get_checksum_check(params,  &tmp_err);
        check_GError(&tmp_err);
        return res;
    }

    void set_src_spacetoken(const std::string & token){
        GError * tmp_err=NULL;
        gfalt_set_src_spacetoken(params, token.c_str(), &tmp_err);
        check_GError(&tmp_err);
    }


    std::string get_src_spacetoken(){
        GError * tmp_err=NULL;
        gchar* res = gfalt_get_dst_spacetoken(params,  &tmp_err);
        check_GError(&tmp_err);
        return (res)?res:"";
    }

    void set_dst_spacetoken(const std::string & token){
        GError * tmp_err=NULL;
        gfalt_set_dst_spacetoken(params, token.c_str(), &tmp_err);
        check_GError(&tmp_err);
    }

    std::string get_dst_spacetoken(){
        GError * tmp_err=NULL;
        gchar* res = gfalt_get_dst_spacetoken(params,  &tmp_err);
        check_GError(&tmp_err);
        return (res)?res:"";
    }


    void set_user_defined_checksum(const std::string & chk_type, const std::string & checksum){
        GError * tmp_err=NULL;
        gfalt_set_user_defined_checksum(params, chk_type.c_str(), checksum.c_str(),&tmp_err);
        check_GError(&tmp_err);
    }

    boost::python::tuple get_user_defined_checksum(){
        char buff_chktype[GFAL_URL_MAX_LEN];
        char buff_chk[GFAL_URL_MAX_LEN];
        GError * tmp_err=NULL;
        gfalt_get_user_defined_checksum(params, buff_chktype, GFAL_URL_MAX_LEN,
                                            buff_chk, GFAL_URL_MAX_LEN, &tmp_err);
        check_GError(&tmp_err);
        return boost::python::make_tuple<std::string,std::string>(buff_chktype, buff_chk);
    }

    void set_overwrite(bool overwrite) {
        GError * tmp_err = NULL;
        gfalt_set_replace_existing_file(params, overwrite, &tmp_err);
        check_GError(&tmp_err);
    }

    bool get_overwrite(void) {
        GError * tmp_err = NULL;
        gboolean overwrite = gfalt_get_replace_existing_file(params, &tmp_err);
        check_GError(&tmp_err);
        return overwrite;
    }

    void set_create_parent(bool create_parent) {
        GError * tmp_err = NULL;
        gfalt_set_create_parent_dir(params, create_parent, &tmp_err);
        check_GError(&tmp_err);
    }

    bool get_create_parent(void) {
        GError * tmp_err = NULL;
        gboolean create = gfalt_get_create_parent_dir(params, &tmp_err);
        check_GError(&tmp_err);
        return create;
    }

    void set_tcp_buffersize(uint64_t buffersize) {
        GError * tmp_err = NULL;
        gfalt_set_tcp_buffer_size(params, buffersize, &tmp_err);
        check_GError(&tmp_err);
    }

    uint64_t get_tcp_buffersize(void) {
        GError * tmp_err = NULL;
        uint64_t buffersize = gfalt_get_tcp_buffer_size(params, &tmp_err);
        check_GError(&tmp_err);
        return buffersize;
    }

    // Callbacks
    void set_event_callback(PyObject* callable) {
        callback_objs.event_callback = boost::python::object(boost::python::handle<>(callable));
        gfalt_set_event_callback(params, event_callback_wrapper, NULL);
        gfalt_set_user_data(params, &callback_objs, NULL);
    }

    PyObject* get_event_callback(void) {
        return callback_objs.event_callback.ptr();
    }

    void set_monitor_callback(PyObject* callable) {
        callback_objs.monitor_callback = boost::python::object(boost::python::handle<>(callable));
        gfalt_set_monitor_callback(params, monitor_callback_wrapper, NULL);
        gfalt_set_user_data(params, &callback_objs, NULL);
    }

    PyObject* get_monitor_callback(void) {
        return callback_objs.monitor_callback.ptr();
    }

    friend class Gfal;
};

#endif // GFALT_PARAMS_H
