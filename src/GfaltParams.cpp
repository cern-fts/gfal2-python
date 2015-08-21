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

#include "GfaltParams.h"
#include "pyGIL.h"

using namespace PyGfal2;


static void event_callback_wrapper(const gfalt_event_t e, gpointer user_data)
{
	ScopedGILLocker lock;
    CallbackObjs* callbacks = static_cast<CallbackObjs*>(user_data);
    if (callbacks->event_callback) {
        boost::python::call<void>(callbacks->event_callback.ptr(), GfaltEvent(e));
    }
}



static void monitor_callback_wrapper(gfalt_transfer_status_t h, const char* src,
        const char* dst, gpointer user_data)
{
	ScopedGILLocker lock;
    CallbackObjs* callbacks = static_cast<CallbackObjs*>(user_data);
    if (callbacks->monitor_callback) {
        size_t avg     = gfalt_copy_get_average_baudrate(h, NULL);
        size_t inst    = gfalt_copy_get_instant_baudrate(h, NULL);
        size_t trans   = gfalt_copy_get_bytes_transfered(h, NULL);
        time_t elapsed = gfalt_copy_get_elapsed_time(h, NULL);

        boost::python::call<void>(callbacks->monitor_callback.ptr(), src, dst, avg, inst, trans, elapsed);
    }
}


GfaltParams::GfaltParams()
{
    GError * tmp_err = NULL;
    params = gfalt_params_handle_new(&tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


GfaltParams::GfaltParams(const GfaltParams & s)
{
    GError * tmp_err = NULL;
    params = gfalt_params_handle_copy(s.params, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


GfaltParams GfaltParams::copy()
{
    return GfaltParams(*this);
}


GfaltParams::~GfaltParams()
{
    gfalt_params_handle_delete(params, NULL);
}


void GfaltParams::set_timeout(guint64 timeout)
{
    GError * tmp_err = NULL;
    gfalt_set_timeout(params, timeout, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


guint64 GfaltParams::get_timeout()
{
    GError * tmp_err = NULL;
    guint64 res = gfalt_get_timeout(params, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return res;
}


void GfaltParams::set_nbstream(guint nbstream)
{
    GError * tmp_err = NULL;
    gfalt_set_nbstreams(params, nbstream, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


guint GfaltParams::get_nbstream()
{
    GError * tmp_err = NULL;
    guint res = gfalt_get_nbstreams(params, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return res;
}


void GfaltParams::set_checksum_check(bool checksum_check)
{
    GError * tmp_err = NULL;
    gfalt_set_checksum_check(params, checksum_check, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


bool GfaltParams::get_checksum_check()
{
    GError * tmp_err = NULL;
    bool res = gfalt_get_checksum_check(params, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return res;
}


void GfaltParams::set_src_spacetoken(const std::string & token)
{
    GError * tmp_err = NULL;
    gfalt_set_src_spacetoken(params, token.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


std::string GfaltParams::get_src_spacetoken()
{
    GError * tmp_err = NULL;
    const gchar* res = gfalt_get_dst_spacetoken(params, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return (res) ? res : "";
}


void GfaltParams::set_dst_spacetoken(const std::string & token)
{
    GError * tmp_err = NULL;
    gfalt_set_dst_spacetoken(params, token.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


std::string GfaltParams::get_dst_spacetoken()
{
    GError * tmp_err = NULL;
    const gchar* res = gfalt_get_dst_spacetoken(params, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return (res) ? res : "";
}


void GfaltParams::set_user_defined_checksum(const std::string & chk_type,
        const std::string & checksum)
{
    GError * tmp_err = NULL;
    gfalt_set_user_defined_checksum(params, chk_type.c_str(), checksum.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


boost::python::tuple GfaltParams::get_user_defined_checksum()
{
    char buff_chktype[GFAL_URL_MAX_LEN];
    char buff_chk[GFAL_URL_MAX_LEN];
    GError * tmp_err = NULL;
    gfalt_get_user_defined_checksum(params, buff_chktype, GFAL_URL_MAX_LEN, buff_chk,
            GFAL_URL_MAX_LEN, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return boost::python::make_tuple<std::string, std::string>(buff_chktype, buff_chk);
}


void GfaltParams::set_overwrite(bool overwrite)
{
    GError * tmp_err = NULL;
    gfalt_set_replace_existing_file(params, overwrite, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


bool GfaltParams::get_overwrite(void)
{
    GError * tmp_err = NULL;
    gboolean overwrite = gfalt_get_replace_existing_file(params, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return overwrite;
}


void GfaltParams::set_create_parent(bool create_parent)
{
    GError * tmp_err = NULL;
    gfalt_set_create_parent_dir(params, create_parent, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


bool GfaltParams::get_create_parent(void)
{
    GError * tmp_err = NULL;
    gboolean create = gfalt_get_create_parent_dir(params, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return create;
}


void GfaltParams::set_tcp_buffersize(uint64_t buffersize)
{
    GError * tmp_err = NULL;
    gfalt_set_tcp_buffer_size(params, buffersize, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


uint64_t GfaltParams::get_tcp_buffersize(void)
{
    GError * tmp_err = NULL;
    uint64_t buffersize = gfalt_get_tcp_buffer_size(params, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return buffersize;
}


bool GfaltParams::get_strict_copy(void)
{
    GError * tmp_err = NULL;
    bool strict = gfalt_get_strict_copy_mode(params, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return strict;
}


void GfaltParams::set_strict_copy(bool val)
{
    GError * tmp_err = NULL;
    gfalt_set_strict_copy_mode(params, val, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}

// Callbacks
void GfaltParams::set_event_callback(PyObject* callable)
{
    callback_objs.event_callback = boost::python::object(
            boost::python::handle<>(callable));
    GError *tmp_err = NULL;
    gfalt_add_event_callback(params, event_callback_wrapper,
            &callback_objs, NULL, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}


PyObject* GfaltParams::get_event_callback(void)
{
    return callback_objs.event_callback.ptr();
}


void GfaltParams::set_monitor_callback(PyObject* callable)
{
    callback_objs.monitor_callback = boost::python::object(
            boost::python::handle<>(callable));
    GError *tmp_err = NULL;
    gfalt_add_monitor_callback(params, monitor_callback_wrapper,
            &callback_objs, NULL, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
}

PyObject* GfaltParams::get_monitor_callback(void)
{
    return callback_objs.monitor_callback.ptr();
}
