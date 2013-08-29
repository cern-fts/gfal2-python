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


static PyObject *GErrorPyType = NULL;



void gerror_exception_translator(const GErrorWrapper  & x){
	assert(GErrorPyType != NULL); // check if type init is valid
    PyErr_SetObject(GErrorPyType, Py_BuildValue("si", x.what(), x.code()));
}


BOOST_PYTHON_MODULE(gfal2)
{
    scope gfal2Scope = scope();

    // global functions
    def("set_verbose", &gfal_set_verbose_enum, "define the log level of gfal 2.0 ");

    enum_<gfal_verbose_levels>("verbose_level")
            .value("normal",gfal_verbose_normal)
            .value("verbose", gfal_verbose_verbose)
            .value("debug", gfal_verbose_debug)
            .value("trace", gfal_verbose_trace)
            ;
	
	// register exception
	GErrorPyType = createGErrorException(gfal2Scope);

    scope scope_posix =  class_<Gfal>("creat_context")
	.def("open", &Gfal::open)

    .def("file", &Gfal::file)
	
    .def("access", &Gfal::access)
    
    .def("lstat", &Gfal::lstat)
    
    .def("stat", &Gfal::stat_c)
    
    .def("chmod", &Gfal::chmod)
    
    .def("unlink", &Gfal::unlink)
    
    .def("mkdir", &Gfal::mkdir)
    
    .def("rmdir", &Gfal::rmdir)
    
    .def("listdir", &Gfal::listdir)
    
    .def("rename", &Gfal::rename)
    
    .def("readlink", &Gfal::readlink)
    
    .def("symlink", &Gfal::symlink)

    .def("checksum", static_cast<std::string (Gfal::*)(const std::string & uri, const std::string & chk_type,
                                              off_t start_offset, size_t data_length)>(&Gfal::checksum))
    
    .def("checksum", static_cast<std::string (Gfal::*)(const std::string & uri, const std::string & chk_type)>(&Gfal::checksum))

    .def("getxattr", &Gfal::getxattr)
    
    .def("setxattr", &Gfal::setxattr)
    
    .def("listxattr", &Gfal::listxattr)

    .def("get_opt_integer", &Gfal::get_opt_integer)

    .def("get_opt_boolean", &Gfal::get_opt_boolean)

    .def("get_opt_string", &Gfal::get_opt_string)

    .def("get_opt_string_list", &Gfal::get_opt_string_list)

    .def("set_opt_string_list", &Gfal::set_opt_string_list)

    .def("set_opt_string", &Gfal::set_opt_string)

    .def("set_opt_boolean", &Gfal::set_opt_boolean)

    .def("set_opt_integer", &Gfal::set_opt_integer)

    .def("filecopy", static_cast<int (Gfal::*)(const std::string & src, const std::string & dst)>(&Gfal::filecopy))

    .def("filecopy", static_cast<int (Gfal::*)(const Gfalt_params& p, const std::string & src, const std::string & dst)>(&Gfal::filecopy))

    .def("cancel", &Gfal::cancel);
    ;

    // register stat struct
    class_<Gfal::Gstat>("st_stat")
		.add_property("st_dev", &Gfal::Gstat::get_st_dev)
		.add_property("st_ino", &Gfal::Gstat::get_st_ino)
		.add_property("st_mode", &Gfal::Gstat::get_st_mode)
		.add_property("st_nlink", &Gfal::Gstat::get_st_nlink)
		.add_property("st_atime", &Gfal::Gstat::get_st_atime)
		.add_property("st_mtime", &Gfal::Gstat::get_st_mtime)
		.add_property("st_ctime", &Gfal::Gstat::get_st_ctime)
		.add_property("st_gid", &Gfal::Gstat::get_st_gid)
		.add_property("st_uid", &Gfal::Gstat::get_st_uid)
		.add_property("st_size", &Gfal::Gstat::get_st_size)
		
		.def("__str__", &Gfal::Gstat::string_rep)
		.def("__repr__", &Gfal::Gstat::string_rep)
	;

    // Transfer parameters
    class_<Gfalt_params >("transfer_parameters")
        .def("copy", &Gfalt_params::copy)
        .add_property("timeout", &Gfalt_params::get_timeout, &Gfalt_params::set_timeout)
        .add_property("checksum_check", &Gfalt_params::get_checksum_check, &Gfalt_params::set_checksum_check)
        .add_property("src_spacetoken", &Gfalt_params::get_src_spacetoken, &Gfalt_params::set_src_spacetoken)
        .add_property("dst_spacetoken", &Gfalt_params::get_dst_spacetoken, &Gfalt_params::set_dst_spacetoken)
        .add_property("nbstreams", &Gfalt_params::get_nbstream, &Gfalt_params::set_nbstream)
        .add_property("overwrite", &Gfalt_params::get_overwrite, &Gfalt_params::set_overwrite)
        .add_property("event_callback", &Gfalt_params::get_event_callback, &Gfalt_params::set_event_callback)
        .add_property("monitor_callback", &Gfalt_params::get_monitor_callback, &Gfalt_params::set_monitor_callback)

        .def("set_user_defined_checksum", &Gfalt_params::set_user_defined_checksum)
        .def("get_user_defined_checksum", &Gfalt_params::get_user_defined_checksum)
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
    class_<Gfal::GfalFile, boost::shared_ptr<Gfal::GfalFile> >("FileType",  init<Gfal, const std::string &, const std::string &>())
        .def("read", &Gfal::GfalFile::read)  // Add a regular member function.
        .def("pread", &Gfal::GfalFile::pread)
        .def("write", &Gfal::GfalFile::write)  
        .def("pwrite", &Gfal::GfalFile::pwrite)
        .def("lseek", &Gfal::GfalFile::lseek)
    ;

}


