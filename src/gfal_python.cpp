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


/**
 * main file for boost python wrapper
 *
 * */


#include <iostream>
#include <string>
#include <limits.h>
#include "gfal_boost_include.hpp"
#include <gfal_api.h>

#include "GErrorWrapper.h"
#include "gfalfile.h"
#include "gfalt_params.h"
#include "gfalcpp.h"


using namespace boost::python;


PyObject *GErrorPyType = NULL;



void gerror_exception_translator(const GErrorWrapper  & x) {
	assert(GErrorPyType != NULL); // check if type init is valid
    PyErr_SetObject(GErrorPyType, Py_BuildValue("si", x.what(), x.code()));
}


Gfal creat_context(void) {
    return Gfal();
}

std::string gfal_version_wrapper(void)
{
    return gfal_version();
}

BOOST_PYTHON_MODULE(gfal2)
{
	// initialize multi-threading
	PyEval_InitThreads();

    scope gfal2Scope = scope();

    // disable boost::python automatic docstring generation style
    // do not confuse users with C++ signatures and the like
    docstring_options local_docstring_options(true, true, false);

    // global functions
    def("set_verbose", &gfal_set_verbose_enum, "Define the log level of gfal2");
    def("creat_context", &creat_context, "Create a gfal2 context");
    def("get_version", &gfal_version_wrapper, "Get the gfal2 version");

    enum_<gfal_verbose_levels>("verbose_level")
            .value("normal", gfal_verbose_normal)
            .value("verbose", gfal_verbose_verbose)
            .value("debug", gfal_verbose_debug)
            .value("trace", gfal_verbose_trace)
            ;

	// register exception
	GErrorPyType = createGErrorException(gfal2Scope);

    scope scope_posix =  class_<Gfal>("Gfal2Context", "Gfal2 Context")
    .def("open", &Gfal::open,
        "Opens a file and returns a file descriptor"
    )
    .def("file", &Gfal::file,
        "Synonym for open"
    )
    .def("opendir", &Gfal::opendir,
        "Opens a directory and returns a directory descriptor"
    )
    .def("directory", &Gfal::directory,
        "Synonym for opendir"
    )
    .def("access", &Gfal::access,
        "Checks if the calling process/user can access the file with the given mode (see os.access)"
    )
    .def("lstat", &Gfal::lstat,
        "Identical to stat, but following symlinks"
    )
    .def("stat", &Gfal::stat_c,
        "Performs a stat call on a file"
    )
    .def("chmod", &Gfal::chmod,
        "Changes the permissions of the file"
    )
    .def("unlink", &Gfal::unlink,
        "Removes a file"
    )
    .def("mkdir", &Gfal::mkdir,
        "Creates a directory"
    )
    .def("mkdir_rec", &Gfal::mkdir_rec,
        "Creates a directory and its parents if needed"
    )
    .def("rmdir", &Gfal::rmdir,
        "Removes a directory"
    )
    .def("listdir", &Gfal::listdir,
        "Returns the content of a directory as a list of file names"
    )
    .def("rename", &Gfal::rename,
        "Renames a file"
    )
    .def("readlink", &Gfal::readlink,
        "Returns a string representing the path to which the symbolic link points."
    )
    .def("symlink", &Gfal::symlink,
        "Creates a symbolic link"
    )
    .def("checksum",
        static_cast<std::string (Gfal::*)(const std::string & uri, const std::string & chk_type, off_t offset, size_t length)>(&Gfal::checksum),
        "Returns the checksum for the given part of a file. If both offset and length are 0, the whole file will be processed"
     )
    .def("checksum", static_cast<std::string (Gfal::*)(const std::string & uri, const std::string & chk_type)>(&Gfal::checksum),
        "Shortcut for checksum(uri, chk_type, 0, 0)"
    )
    .def("getxattr", &Gfal::getxattr,
        "Gets an extended attribute"
    )
    .def("setxattr", &Gfal::setxattr,
        "Sets an extended attribute"
    )
    .def("listxattr", &Gfal::listxattr,
        "List known/supported extended attributes"
    )
    .def("get_opt_integer", &Gfal::get_opt_integer,
        "Returns the integer value assigned to a configuration parameter"
    )
    .def("get_opt_boolean", &Gfal::get_opt_boolean,
        "Returns the boolean value assigned to a configuration parameter"
    )
    .def("get_opt_string", &Gfal::get_opt_string,
        "Returns the string value assigned to a configuration parameter"
    )
    .def("get_opt_string_list", &Gfal::get_opt_string_list,
        "Returns the list of strings assigned to a configuration parameter"
    )
    .def("set_opt_string_list", &Gfal::set_opt_string_list,
        "Sets a configuration parameter as a list of strings"
    )
    .def("set_opt_string", &Gfal::set_opt_string,
        "Sets a configuration parameter as a string"
    )
    .def("set_opt_boolean", &Gfal::set_opt_boolean,
        "Sets a configuration parameter as a boolean"
    )
    .def("set_opt_integer", &Gfal::set_opt_integer,
        "Sets a configuration parameter as an integer"
    )
    .def("load_opts_from_file", &Gfal::load_opts_from_file,
        "Loads a set of configuration parameters from a .ini formatted file"
    )
    .def("filecopy", static_cast<int (Gfal::*)(const std::string & src, const std::string & dst)>(&Gfal::filecopy),
        "Shortcut for filecopy(gfal2.transfer_params(), src, dst)"
    )
    .def("filecopy", static_cast<int (Gfal::*)(const Gfalt_params& p, const std::string & src, const std::string & dst)>(&Gfal::filecopy),
        "Copies src into dst using the configured transfer parameters"
    )
    .def("filecopy", static_cast<boost::python::object (Gfal::*)(const boost::python::list&, const boost::python::list&)>(&Gfal::filecopy),
        "Shortcut for filecopy(gfal2.transfer_params(), sources, destinations)"
    )
    .def("filecopy", static_cast<boost::python::object (Gfal::*)(const Gfalt_params&, const boost::python::list&, const boost::python::list&)>(&Gfal::filecopy),
        "Shortcut for filecopy(params, sources, destinations, [])"
    )
    .def("filecopy", static_cast<boost::python::object (Gfal::*)(const Gfalt_params&, const boost::python::list&, const boost::python::list&, const boost::python::list&)>(&Gfal::filecopy),
        "Performs a bulk copy from sources[i] to destinations[i] with checksum checksum[i]. All sources must use same protocol/storage, and same goes for destinations."
    )
    .def("cancel", &Gfal::cancel,
        "Cancel running operations")
    .def("bring_online", &Gfal::bring_online,
        "Performs a bring online operation (only for protocols that support this operation)"
    )
    .def("bring_online_poll", &Gfal::bring_online_poll,
        "Polls the status for asynchronous bring_online operations"
    )
    .def("release", &Gfal::release,
        "Releases a set of files pinned by a bring_online call"
    );

    // register stat struct
    class_<Gfal::GStat, boost::shared_ptr<Gfal::GStat> >("Stat", "Please, note that not all fields make sense for all protocols")
        .add_property("st_dev", &Gfal::GStat::get_st_dev, "Device of contatining file")
        .add_property("st_ino", &Gfal::GStat::get_st_ino, "Inode")
        .add_property("st_mode", &Gfal::GStat::get_st_mode, "Protection mode")
        .add_property("st_nlink", &Gfal::GStat::get_st_nlink, "Number of hard links")
        .add_property("st_atime", &Gfal::GStat::get_st_atime, "Access time")
        .add_property("st_mtime", &Gfal::GStat::get_st_mtime, "Modification time")
        .add_property("st_ctime", &Gfal::GStat::get_st_ctime, "Creation time")
        .add_property("st_gid", &Gfal::GStat::get_st_gid, "Group ID")
        .add_property("st_uid", &Gfal::GStat::get_st_uid, "User ID")
        .add_property("st_size", &Gfal::GStat::get_st_size, "Size")

        .def("__str__", &Gfal::GStat::string_rep)
        .def("__repr__", &Gfal::GStat::string_rep)
	;

	// register dirent struct
    class_<Gfal::GDirent, boost::shared_ptr<Gfal::GDirent> >("Dirent", "Please, note that not all fields make sense for all protocols")
        .add_property("d_ino", &Gfal::GDirent::get_d_ino, "Inode")
        .add_property("d_off", &Gfal::GDirent::get_d_off, "Offset to the next dirent")
        .add_property("d_reclen", &Gfal::GDirent::get_d_reclen, "Lenght of this record")
        .add_property("d_type", &Gfal::GDirent::get_d_type, "Type of file")
        .add_property("d_name", &Gfal::GDirent::get_d_name, "Entry name")
        .def("__nonzero__", &Gfal::GDirent::isValid)
	;

    // Transfer parameters
    class_<Gfalt_params>("transfer_parameters", "filecopy parameters")
        .def("copy", &Gfalt_params::copy)
        .add_property("timeout", &Gfalt_params::get_timeout, &Gfalt_params::set_timeout,
            "Operation timeout"
        )
        .add_property("checksum_check", &Gfalt_params::get_checksum_check, &Gfalt_params::set_checksum_check,
            "Perform checksum validation"
        )
        .add_property("src_spacetoken", &Gfalt_params::get_src_spacetoken, &Gfalt_params::set_src_spacetoken,
            "Source spacetoken (for protocols that support it)"
        )
        .add_property("dst_spacetoken", &Gfalt_params::get_dst_spacetoken, &Gfalt_params::set_dst_spacetoken,
            "Destination spacetoken (for protocols that support it)"
        )
        .add_property("nbstreams", &Gfalt_params::get_nbstream, &Gfalt_params::set_nbstream,
            "Number of streams"
        )
        .add_property("overwrite", &Gfalt_params::get_overwrite, &Gfalt_params::set_overwrite,
            "If the destination file exists, overwrite it"
        )
        .add_property("create_parent", &Gfalt_params::get_create_parent, &Gfalt_params::set_create_parent,
            "If the destination parent directory does not exist, create it"
        )
        .add_property("tcp_buffersize", &Gfalt_params::get_tcp_buffersize, &Gfalt_params::set_tcp_buffersize,
            "TCP Buffersize"
        )
        .add_property("strict_copy", &Gfalt_params::get_strict_copy, &Gfalt_params::set_strict_copy,
            "If set to True, do only copy, and ignore checksum, parent creation, overwrite...")
        .add_property("event_callback", &Gfalt_params::get_event_callback, &Gfalt_params::set_event_callback,
            "Callback for event handling"
        )
        .add_property("monitor_callback", &Gfalt_params::get_monitor_callback, &Gfalt_params::set_monitor_callback,
            "Callback for performance monitoring (i.e. throughput)"
        )

        .def("set_user_defined_checksum", &Gfalt_params::set_user_defined_checksum,
            "Specify manually the checksum type and value (optional)")
        .def("get_user_defined_checksum", &Gfalt_params::get_user_defined_checksum,
            "Gets the user specified checksum")
        ;

    // Callback types
    enum_<gfal_event_side_t>("event_side")
        .value("event_source", GFAL_EVENT_SOURCE)
        .value("event_destination", GFAL_EVENT_DESTINATION)
        .value("event_none", GFAL_EVENT_NONE)
        ;

    class_<Gfalt_event>("gfalt_event")
        .add_property("side", &Gfalt_event::side)
        .add_property("timestamp", &Gfalt_event::timestamp)
        .add_property("stage", &Gfalt_event::stage)
        .add_property("domain", &Gfalt_event::domain)
        .add_property("description", &Gfalt_event::description)

        .def("__str__", &Gfalt_event::__str__)
        .def("__repr__", &Gfalt_event::__str__)
        ;

    // register exception
    register_exception_translator<GErrorWrapper>(&gerror_exception_translator);

    // Create the Python type object for our extension class and define __init__ function.
    class_<Gfal::GfalFile, boost::shared_ptr<Gfal::GfalFile>, boost::noncopyable >("FileType", "File descriptor", init<Gfal, const std::string &, const std::string &>())
        .def("read", &Gfal::GfalFile::read)
        .def("pread", &Gfal::GfalFile::pread)
        .def("write", &Gfal::GfalFile::write)
        .def("pwrite", &Gfal::GfalFile::pwrite)
        .def("lseek", &Gfal::GfalFile::lseek)
    ;

    class_<Gfal::GfalDirectory, boost::shared_ptr<Gfal::GfalDirectory>, boost::noncopyable >("DirectoryType", "Directory descriptor", init<Gfal, const std::string &>())
        .def("read", &Gfal::GfalDirectory::read,
            "Reads a directory entry from the directory"
        )
        .def("readpp", &Gfal::GfalDirectory::readpp,
            "Reads a directory entry and its stat information")
    ;

}


