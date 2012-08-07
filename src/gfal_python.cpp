/*
 * Copyright (c) Members of the EGEE Collaboration. 2004.
 * See http://www.eu-egee.org/partners/ for details on the copyright holders.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
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

#include "gerror_exception.h"
#include "gfalfile.h"
#include "gfal_stat.h"
#include "gfalt_params.h"
#include "gfalcpp.h"




using namespace boost::python;


static PyObject *GErrorPyType = NULL;


void gerror_exception_translator(const Gerror_exception  & x){
	assert(GErrorPyType != NULL); // check if type init is valid
	object pythonGErrorInstance(x);
    PyErr_SetObject(GErrorPyType, pythonGErrorInstance.ptr());
}



BOOST_PYTHON_MODULE(gfal2)
{

    // global functions
    def("set_verbose", &gfal_set_verbose_enum, "define the log level of gfal 2.0 ");

    enum_<gfal_verbose_levels>("verbose_level")
            .value("normal",gfal_verbose_normal)
            .value("verbose", gfal_verbose_verbose)
            .value("debug", gfal_verbose_debug)
            .value("trace", gfal_verbose_trace)
            ;
	
	// register exception first 
	class_<Gerror_exception> pyGErrorException("GError", init<const std::string &, int>()); 
	pyGErrorException.def("message", &Gerror_exception::get_message);
	pyGErrorException.def("code", &Gerror_exception::code);
	pyGErrorException.def("__str__", &Gerror_exception::get_message);
		


	GErrorPyType = pyGErrorException.ptr();
	
    scope scope_posix =  class_<Gfal>("creat_context")
	.def("open", &Gfal::open)
	
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

    class_<Gfalt_params >("transfer_parameters")
            .def("copy", &Gfalt_params::copy)
            .add_property("timeout", &Gfalt_params::get_timeout, &Gfalt_params::set_timeout)
            .add_property("checksum_check", &Gfalt_params::get_checksum_check, &Gfalt_params::set_checksum_check)
            .add_property("src_spacetoken", &Gfalt_params::get_src_spacetoken, &Gfalt_params::set_src_spacetoken)
            .add_property("dst_spacetoken", &Gfalt_params::get_dst_spacetoken, &Gfalt_params::set_dst_spacetoken)
            .add_property("nbstreams", &Gfalt_params::get_nbstream, &Gfalt_params::set_nbstream)

            .def("set_user_defined_checksum", &Gfalt_params::set_user_defined_checksum)
            .def("get_user_defined_checksum", &Gfalt_params::get_user_defined_checksum)
            ;
	
    // register exception
    register_exception_translator<Gerror_exception>(&gerror_exception_translator);
    // Create the Python type object for our extension class and define __init__ function.
    class_<Gfal::GfalFile, boost::shared_ptr<Gfal::GfalFile> >("file",  init<const std::string &, const std::string &>())
        .def("read", &Gfal::GfalFile::read)  // Add a regular member function.
        .def("write", &Gfal::GfalFile::write)  
        .def("lseek", &Gfal::GfalFile::lseek)
        
    ;
    


}


