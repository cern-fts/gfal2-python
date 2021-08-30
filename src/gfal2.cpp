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

#include <boost/python.hpp>
#include <iostream>
#include <string>

#include <gfal_api.h>

#include "Gfal2Context.h"
#include "GfaltParams.h"
#include "LoggingHelper.h"
#include "pyGIL.h"


void gerror_exception_translator(const PyGfal2::GErrorWrapper &x)
{
    assert(PyGfal2::GErrorPyType != NULL);
    PyErr_SetObject(PyGfal2::GErrorPyType, Py_BuildValue("si", x.what(), x.code()));
}


std::string gfal_version_wrapper(void)
{
    return gfal2_version();
}

// Wrapper to register the ptr without generating warnings with older
// boost versions
template <typename T>
void register_shared_ptr(void)
{
    const boost::python::type_info info = boost::python::type_id<boost::shared_ptr < T > >();
    const boost::python::converter::registration* reg = boost::python::converter::registry::query(info);

    if (reg == NULL || reg->m_to_python == NULL) {
        boost::python::register_ptr_to_python < boost::shared_ptr < T > > ();
    }
}

boost::shared_ptr<PyGfal2::Cred> gfal2_cred_new_wrapper(const std::string& type, const std::string& value)
{
    fprintf(stderr, "Deprecated: Please use context.cred_new() instead!\n");
    return boost::shared_ptr<PyGfal2::Cred>(new PyGfal2::Cred(type, value));
}

int gfal2_cred_set_wrapper(PyGfal2::Gfal2Context* ctx, const std::string& url_prefix, const PyGfal2::Cred& cred)
{
    fprintf(stderr, "Deprecated: Please use context.cred_set() instead!\n");
    return ctx->cred_set(url_prefix, cred);
}

int gfal2_cred_clean_wrapper(PyGfal2::Gfal2Context* ctx)
{
    fprintf(stderr, "Deprecated: Please use context.cred_clean() instead!\n");
    return ctx->cred_clean();
}

BOOST_PYTHON_MODULE (gfal2)
{
    Py_Initialize();

#if PY_VERSION_HEX < 0x3070000
    // initialize multi-threading, unused since Python 3.7
    PyEval_InitThreads();
#endif

    boost::python::scope gfal2Scope = boost::python::scope();

    // Setup the logging
    gfal2_log_set_handler(&PyGfal2::logging_helper, NULL);
    gfal2_log_set_level(G_LOG_LEVEL_INFO);

    // disable boost::python automatic docstring generation style
    // do not confuse users with C++ signatures and the like
    boost::python::docstring_options local_docstring_options(true, true, false);

    // Expose gfal2-python version
#ifdef GFAL2_PYTHON_VERSION
    boost::python::scope().attr("__version__") = boost::python::str(GFAL2_PYTHON_VERSION);
#endif

    // global functions
    boost::python::def("set_verbose", &PyGfal2::gfal_set_verbose_enum, "Define the log level of gfal2");
    boost::python::def("creat_context", &PyGfal2::Gfal2Context::creat_context, "Create a gfal2 context");
    boost::python::def("get_version", &gfal_version_wrapper, "Get the gfal2 version");

    // Credential object wrappers - kept to provide backwards compatibility
    // (deprecated: new versions should use Gfal2Context methods)
    boost::python::def("cred_set", &gfal2_cred_set_wrapper, "Set credentials");
    boost::python::def("cred_new", &gfal2_cred_new_wrapper, "Define credentials");
    boost::python::def("cred_clean", &gfal2_cred_clean_wrapper, "Clean credentials");

    boost::python::enum_<GLogLevelFlags>("verbose_level")
        .value("normal", G_LOG_LEVEL_CRITICAL)
        .value("warning", G_LOG_LEVEL_WARNING)
        .value("verbose", G_LOG_LEVEL_INFO)
        .value("debug", G_LOG_LEVEL_DEBUG)
        .value("trace", G_LOG_LEVEL_DEBUG);

    boost::python::enum_<gfalt_checksum_mode_t>("checksum_mode")
        .value("none", GFALT_CHECKSUM_NONE)
        .value("source", GFALT_CHECKSUM_SOURCE)
        .value("target", GFALT_CHECKSUM_TARGET)
        .value("both", GFALT_CHECKSUM_BOTH);

    // register exception
    PyGfal2::GErrorPyType = PyGfal2::createGErrorExceptionType(gfal2Scope);

    boost::python::scope context_scope = boost::python::class_<PyGfal2::Gfal2Context>("Gfal2Context", "Gfal2 Context")
        .def("free", &PyGfal2::Gfal2Context::free,
            "Release internal resources used by the context. The instance can *not* be used after this"
        )
        .def("open", &PyGfal2::Gfal2Context::open,
            "Opens a file and returns a file descriptor"
        )
        .def("file", &PyGfal2::Gfal2Context::file,
            "Synonym for open"
        )
        .def("opendir", &PyGfal2::Gfal2Context::opendir,
            "Opens a directory and returns a directory descriptor"
        )
        .def("directory", &PyGfal2::Gfal2Context::directory,
            "Synonym for opendir"
        )
        .def("access", &PyGfal2::Gfal2Context::access,
            "Checks if the calling process/user can access the file with the given mode (see os.access)"
        )
        .def("lstat", &PyGfal2::Gfal2Context::lstat,
            "Identical to stat, but following symlinks"
        )
        .def("stat", &PyGfal2::Gfal2Context::stat_c,
            "Performs a stat call on a file"
        )
        .def("chmod", &PyGfal2::Gfal2Context::chmod,
            "Changes the permissions of the file"
        )
        .def("unlink", &PyGfal2::Gfal2Context::unlink,
            "Removes a file"
        )
        .def("unlink", &PyGfal2::Gfal2Context::unlink_list,
            "Removes a set of files"
        )
        .def("mkdir", &PyGfal2::Gfal2Context::mkdir,
            "Creates a directory"
        )
        .def("mkdir_rec", &PyGfal2::Gfal2Context::mkdir_rec,
            "Creates a directory and its parents if needed"
        )
        .def("rmdir", &PyGfal2::Gfal2Context::rmdir,
            "Removes a directory"
        )
        .def("listdir", &PyGfal2::Gfal2Context::listdir,
            "Returns the content of a directory as a list of file names"
        )
        .def("rename", &PyGfal2::Gfal2Context::rename,
            "Renames a file"
        )
        .def("readlink", &PyGfal2::Gfal2Context::readlink,
            "Returns a string representing the path to which the symbolic link points."
        )
        .def("symlink", &PyGfal2::Gfal2Context::symlink,
            "Creates a symbolic link"
        )
        .def("checksum",
            static_cast<std::string (PyGfal2::Gfal2Context::*)(const std::string &uri, const std::string &chk_type,
                off_t offset, size_t length)>(&PyGfal2::Gfal2Context::checksum),
            "Returns the checksum for the given part of a file. If both offset and length are 0, the whole file will be processed"
        )
        .def("checksum", static_cast<std::string (PyGfal2::Gfal2Context::*)(const std::string &uri,
                const std::string &chk_type)>(&PyGfal2::Gfal2Context::checksum),
            "Shortcut for checksum(uri, chk_type, 0, 0)"
        )
        .def("getxattr", &PyGfal2::Gfal2Context::getxattr,
            "Gets an extended attribute"
        )
        .def("setxattr", &PyGfal2::Gfal2Context::setxattr,
            "Sets an extended attribute"
        )
        .def("listxattr", &PyGfal2::Gfal2Context::listxattr,
            "List known/supported extended attributes"
        )
	      .def("remove_opt", &PyGfal2::Gfal2Context::remove_opt,
            "remove an option"
	      )
        .def("get_opt_integer", &PyGfal2::Gfal2Context::get_opt_integer,
            "Returns the integer value assigned to a configuration parameter"
        )
        .def("get_opt_boolean", &PyGfal2::Gfal2Context::get_opt_boolean,
            "Returns the boolean value assigned to a configuration parameter"
        )
        .def("get_opt_string", &PyGfal2::Gfal2Context::get_opt_string,
            "Returns the string value assigned to a configuration parameter"
        )
        .def("get_opt_string_list", &PyGfal2::Gfal2Context::get_opt_string_list,
            "Returns the list of strings assigned to a configuration parameter"
        )
        .def("set_opt_string_list", &PyGfal2::Gfal2Context::set_opt_string_list,
            "Sets a configuration parameter as a list of strings"
        )
        .def("set_opt_string", &PyGfal2::Gfal2Context::set_opt_string,
            "Sets a configuration parameter as a string"
        )
        .def("set_opt_boolean", &PyGfal2::Gfal2Context::set_opt_boolean,
            "Sets a configuration parameter as a boolean"
        )
        .def("set_opt_integer", &PyGfal2::Gfal2Context::set_opt_integer,
            "Sets a configuration parameter as an integer"
        )
        .def("load_opts_from_file", &PyGfal2::Gfal2Context::load_opts_from_file,
            "Loads a set of configuration parameters from a .ini formatted file"
        )
        .def("set_user_agent", &PyGfal2::Gfal2Context::set_user_agent,
            "Sets the user agent identification, name and version"
        )
        .def("get_user_agent", &PyGfal2::Gfal2Context::get_user_agent,
            "Gets the user agent identification, name and version"
        )
        .def("add_client_info", &PyGfal2::Gfal2Context::add_client_info,
            "Sets a custom key/value pair to be sent to the server, if the protocol allows it"
        )
        .def("remove_client_info", &PyGfal2::Gfal2Context::remove_client_info,
            "Removes a key/value pair set previously by add_client_info"
        )
        .def("clear_client_info", &PyGfal2::Gfal2Context::clear_client_info,
            "Clears all key/value pairs set by add_client_info"
        )
        .def("get_client_info", &PyGfal2::Gfal2Context::get_client_info,
            "Returns the key/value pairs as a dictionary"
        )
        .def("filecopy", static_cast<int (PyGfal2::Gfal2Context::*)(const std::string &src,
                const std::string &dst)>(&PyGfal2::Gfal2Context::filecopy),
            "Shortcut for filecopy(gfal2.transfer_params(), src, dst)"
        )
        .def("filecopy",
            static_cast<int (PyGfal2::Gfal2Context::*)(const PyGfal2::GfaltParams &p, const std::string &src,
                const std::string &dst)>(&PyGfal2::Gfal2Context::filecopy),
            "Copies src into dst using the configured transfer parameters"
        )
        .def("filecopy",
            static_cast<boost::python::object (PyGfal2::Gfal2Context::*)(const boost::python::list &,
                const boost::python::list &)>(&PyGfal2::Gfal2Context::filecopy),
            "Shortcut for filecopy(gfal2.transfer_params(), sources, destinations)"
        )
        .def("filecopy",
            static_cast<boost::python::object (PyGfal2::Gfal2Context::*)(const PyGfal2::GfaltParams &,
                const boost::python::list &, const boost::python::list &)>(&PyGfal2::Gfal2Context::filecopy),
            "Shortcut for filecopy(params, sources, destinations, [])"
        )
        .def("filecopy",
            static_cast<boost::python::object (PyGfal2::Gfal2Context::*)(const PyGfal2::GfaltParams &,
                const boost::python::list &, const boost::python::list &,
                const boost::python::list &)>(&PyGfal2::Gfal2Context::filecopy),
            "Performs a bulk copy from sources[i] to destinations[i] with checksum checksum[i]. All sources must use same protocol/storage, and same goes for destinations."
        )
        .def("cancel", &PyGfal2::Gfal2Context::cancel,
            "Cancel running operations")
        .def("bring_online", &PyGfal2::Gfal2Context::bring_online,
            "Performs a bring online operation (only for protocols that support this operation)"
        )
        .def("bring_online_poll", &PyGfal2::Gfal2Context::bring_online_poll,
            "Polls the status for asynchronous bring_online operations"
        )
        .def("release", &PyGfal2::Gfal2Context::release,
            "Releases a file pinned by a bring_online call"
        )
        .def("bring_online", &PyGfal2::Gfal2Context::bring_online_list,
            "Performs a bulk bring online operation"
        )
        .def("bring_online_poll", &PyGfal2::Gfal2Context::bring_online_poll_list,
            "Performs a bulk bring online poll operation"
        )
        .def("release", &PyGfal2::Gfal2Context::release_list,
            "Releases a set of files pinned by a bring_online call"
        )
        .def("abort_bring_online", &PyGfal2::Gfal2Context::abort_bring_online,
            "Aborts a bring online request")
        .def("abort_bring_online", &PyGfal2::Gfal2Context::abort_bring_online_list,
            "Aborts a bring online request")
        .def("get_plugin_names", &PyGfal2::Gfal2Context::get_plugin_names,
            "Returns the name list of loaded plugins")
        .def("qos_check_classes", &PyGfal2::Gfal2Context::qos_check_classes,
            "Check the QoS classes present in an interface")
        .def("check_file_qos", &PyGfal2::Gfal2Context::check_file_qos,
            "Check the QoS of a file in a CDMI enabled interface")
        .def("check_available_qos_transitions", &PyGfal2::Gfal2Context::check_available_qos_transitions,
            "Check the available transitions for a specific QoS")
        .def("check_target_qos", &PyGfal2::Gfal2Context::check_target_qos,
            "Check the target QoS of a specific file")
        .def("change_object_qos", &PyGfal2::Gfal2Context::change_object_qos,
            "Change the QoS of an object, either dir or file")
        .def("token_retrieve",
             static_cast<std::string (PyGfal2::Gfal2Context::*)(const std::string& url, const std::string& issuer,
                                                                unsigned validity, bool write_access)>
                                                                (&PyGfal2::Gfal2Context::token_retrieve),
             "Retrieve SE-issued token for given resource (uses predefined activities based on read/write access flag)")
        .def("token_retrieve",
             static_cast<std::string (PyGfal2::Gfal2Context::*)(const std::string& url, const std::string& issuer,
                                                                unsigned validity, const boost::python::list &)>
                                                                (&PyGfal2::Gfal2Context::token_retrieve),
             "Retrieve SE-issued token for given resource (uses user-defined activities)")
        .def("cred_set", &PyGfal2::Gfal2Context::cred_set,
            "Set credentials of specified type for given URL")
        .def("cred_get", &PyGfal2::Gfal2Context::cred_get,
            "Get credential of specified type for given URL")
        .def("cred_clean", &PyGfal2::Gfal2Context::cred_clean,
            "Clean credential mapping");

    // register stat struct
    boost::python::class_<PyGfal2::Stat>
        ("Stat", "Please, note that not all fields make sense for all protocols")
        .add_property("st_dev", &PyGfal2::Stat::get_st_dev, "Device of containing file")
        .add_property("st_ino", &PyGfal2::Stat::get_st_ino, "Inode")
        .add_property("st_mode", &PyGfal2::Stat::get_st_mode, "Protection mode")
        .add_property("st_nlink", &PyGfal2::Stat::get_st_nlink, "Number of hard links")
        .add_property("st_atime", &PyGfal2::Stat::get_st_atime, "Access time")
        .add_property("st_mtime", &PyGfal2::Stat::get_st_mtime, "Modification time")
        .add_property("st_ctime", &PyGfal2::Stat::get_st_ctime, "Creation time")
        .add_property("st_gid", &PyGfal2::Stat::get_st_gid, "Group ID")
        .add_property("st_uid", &PyGfal2::Stat::get_st_uid, "User ID")
        .add_property("st_size", &PyGfal2::Stat::get_st_size, "Size")
        .def("__str__", &PyGfal2::Stat::__str__)
        .def("__repr__", &PyGfal2::Stat::__str__);

    // register dirent struct
    boost::python::class_<PyGfal2::Dirent>
        ("Dirent", "Please, note that not all fields make sense for all protocols")
        .add_property("d_ino", &PyGfal2::Dirent::get_d_ino, "Inode")
        .add_property("d_off", &PyGfal2::Dirent::get_d_off, "Offset to the next dirent")
        .add_property("d_reclen", &PyGfal2::Dirent::get_d_reclen, "Length of this record")
        .add_property("d_type", &PyGfal2::Dirent::get_d_type, "Type of file")
        .add_property("d_name", &PyGfal2::Dirent::get_d_name, "Entry name")
        .def("__nonzero__", &PyGfal2::Dirent::isValid);

    // register Gfal2 credential struct
    boost::python::class_<PyGfal2::Cred>
        ("Credential", "Credential object holding type and value",
            boost::python::init<const std::string &, const std::string &>())
        .add_property("type", &PyGfal2::Cred::get_type, "Credential type")
        .add_property("value", &PyGfal2::Cred::get_value, "Credential value");

    register_shared_ptr<PyGfal2::Cred>();

    boost::python::scope().attr("cred_new") = boost::python::scope().attr("Credential");

    // Transfer parameters
    boost::python::class_<PyGfal2::GfaltParams>("TransferParameters", "filecopy parameters")
        .def("copy", &PyGfal2::GfaltParams::copy)
        .add_property("timeout", &PyGfal2::GfaltParams::get_timeout, &PyGfal2::GfaltParams::set_timeout,
            "Operation timeout"
        )
        .add_property("checksum_check", &PyGfal2::GfaltParams::get_checksum_check,
            &PyGfal2::GfaltParams::set_checksum_check,
            "Perform checksum validation"
        )
        .add_property("src_spacetoken", &PyGfal2::GfaltParams::get_src_spacetoken,
            &PyGfal2::GfaltParams::set_src_spacetoken,
            "Source spacetoken (for protocols that support it)"
        )
        .add_property("dst_spacetoken", &PyGfal2::GfaltParams::get_dst_spacetoken,
            &PyGfal2::GfaltParams::set_dst_spacetoken,
            "Destination spacetoken (for protocols that support it)"
        )
        .add_property("nbstreams", &PyGfal2::GfaltParams::get_nbstream, &PyGfal2::GfaltParams::set_nbstream,
            "Number of streams"
        )
        .add_property("overwrite", &PyGfal2::GfaltParams::get_overwrite, &PyGfal2::GfaltParams::set_overwrite,
            "If the destination file exists, overwrite it"
        )
        .add_property("create_parent", &PyGfal2::GfaltParams::get_create_parent,
            &PyGfal2::GfaltParams::set_create_parent,
            "If the destination parent directory does not exist, create it"
        )
        .add_property("tcp_buffersize", &PyGfal2::GfaltParams::get_tcp_buffersize,
            &PyGfal2::GfaltParams::set_tcp_buffersize,
            "TCP Buffersize"
        )
        .add_property("strict_copy", &PyGfal2::GfaltParams::get_strict_copy,
            &PyGfal2::GfaltParams::set_strict_copy,
            "If set to True, do only copy, and ignore checksum, parent creation, overwrite..."
        )
        .add_property("proxy_delegation", &PyGfal2::GfaltParams::get_proxy_delegation,
            &PyGfal2::GfaltParams::set_proxy_delegation,
            "Enable or disable TPC with proxy delegation (default enabled)"
        )
        .add_property("local_transfers", &PyGfal2::GfaltParams::get_local_transfers,
            &PyGfal2::GfaltParams::set_local_transfers,
            "Permission flag to enable or disable transfers via local streaming (default enabled)"
        )
        .add_property("event_callback", &PyGfal2::GfaltParams::get_event_callback,
            &PyGfal2::GfaltParams::set_event_callback,
            "Callback for event handling"
        )
        .add_property("monitor_callback", &PyGfal2::GfaltParams::get_monitor_callback,
            &PyGfal2::GfaltParams::set_monitor_callback,
            "Callback for performance monitoring (i.e. throughput)"
        )

        .def("set_user_defined_checksum", &PyGfal2::GfaltParams::set_user_defined_checksum,
            "Specify manually the checksum type and value (optional)")
        .def("get_user_defined_checksum", &PyGfal2::GfaltParams::get_user_defined_checksum,
            "Get the user specified checksum")

        .def("set_checksum", &PyGfal2::GfaltParams::set_checksum,
            "Specify the checksum mode, type and value")
        .def("get_checksum", &PyGfal2::GfaltParams::get_checksum,
            "Get the user specified checksum mode, type and value")
        ;

    boost::python::scope().attr("transfer_parameters") = boost::python::scope().attr("TransferParameters");

    // Callback types
    boost::python::enum_<gfal_event_side_t>("event_side")
        .value("event_source", GFAL_EVENT_SOURCE)
        .value("event_destination", GFAL_EVENT_DESTINATION)
        .value("event_none", GFAL_EVENT_NONE);

    boost::python::class_<PyGfal2::GfaltEvent>("GfaltEvent")
        .add_property("side", &PyGfal2::GfaltEvent::side)
        .add_property("timestamp", &PyGfal2::GfaltEvent::timestamp)
        .add_property("stage", &PyGfal2::GfaltEvent::stage)
        .add_property("domain", &PyGfal2::GfaltEvent::domain)
        .add_property("description", &PyGfal2::GfaltEvent::description)

        .def("__str__", &PyGfal2::GfaltEvent::__str__)
        .def("__repr__", &PyGfal2::GfaltEvent::__str__);

    boost::python::scope().attr("gfalt_event") = boost::python::scope().attr("GfaltEvent");

    // register exception
    boost::python::register_exception_translator<PyGfal2::GErrorWrapper>(&gerror_exception_translator);

    // Create the Python type object for our extension class and define __init__ function.
    boost::python::class_<PyGfal2::File, boost::shared_ptr<PyGfal2::File>, boost::noncopyable>
        ("FileType", "File descriptor",
            boost::python::init<PyGfal2::Gfal2Context, const std::string &, const std::string &>())
        .def("read", &PyGfal2::File::read)
        .def("pread", &PyGfal2::File::pread)
        .def("read_bytes", &PyGfal2::File::read_bytes)
        .def("pread_bytes", &PyGfal2::File::pread_bytes)
        .def("write", &PyGfal2::File::write)
        .def("pwrite", &PyGfal2::File::pwrite)
        .def("lseek", &PyGfal2::File::lseek);

    register_shared_ptr<PyGfal2::File>();

    boost::python::class_<PyGfal2::Directory, boost::shared_ptr<PyGfal2::Directory>, boost::noncopyable>
        ("DirectoryType", "Directory descriptor", boost::python::init<PyGfal2::Gfal2Context, const std::string &>())
        .def("read", &PyGfal2::Directory::read,
            "Reads a directory entry from the directory")
        .def("readpp", &PyGfal2::Directory::readpp,
            "Reads a directory entry and its stat information");

    register_shared_ptr<PyGfal2::Directory>();

    // Null log handler
    boost::python::class_<PyGfal2::NullHandler> nullHandler("NullHandler");
    nullHandler.def("emit", &PyGfal2::NullHandler::emit)
        .def("createLock", &PyGfal2::NullHandler::createLock)
        .def("acquire", &PyGfal2::NullHandler::acquire)
        .def("release", &PyGfal2::NullHandler::release)
        .def("setLevel", &PyGfal2::NullHandler::setLevel)
        .def("setFormatter", &PyGfal2::NullHandler::setFormatter)
        .def("addFilter", &PyGfal2::NullHandler::addFilter)
        .def("removeFilter", &PyGfal2::NullHandler::removeFilter)
        .def("filter", &PyGfal2::NullHandler::filter)
        .def("flush", &PyGfal2::NullHandler::flush)
        .def("close", &PyGfal2::NullHandler::close)
        .def("handle", &PyGfal2::NullHandler::handle)
        .def("handleError", &PyGfal2::NullHandler::handleError)
        .def("format", &PyGfal2::NullHandler::format)
        .def("emit", &PyGfal2::NullHandler::emit)
        .add_property("level", &PyGfal2::NullHandler::level)
        .add_property("filters", &PyGfal2::NullHandler::filters);

    gfal2Scope.attr("NullHandler") = nullHandler;

    // Register the null handler for gfal2 by default
    PyGfal2::logging_register_handler("gfal2", boost::python::object(nullHandler)());
}
