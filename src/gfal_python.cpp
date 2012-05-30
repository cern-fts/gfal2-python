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


#include "gerror_exception.h"
#include "gfalfile.h"
#include "gfal_stat.h"
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
	
	
	// register exception first 
	
	class_<Gerror_exception> pyGErrorException("GError", init<const std::string &, int>()); 
	pyGErrorException.def("message", &Gerror_exception::get_message);
	pyGErrorException.def("code", &Gerror_exception::code);
	pyGErrorException.def("__str__", &Gerror_exception::get_message);
		


	GErrorPyType = pyGErrorException.ptr();
	
	scope scope_posix =  class_<Gfal>("posix")
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
    
    .def("getxattr", &Gfal::getxattr)
    
    .def("setxattr", &Gfal::setxattr)
    
    .def("listxattr", &Gfal::listxattr)
    
    ;
    
    
	
	// regsiter stat struct
	class_<Gfal::Gstat>("stat")
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
	
    // register exception
    register_exception_translator<Gerror_exception>(&gerror_exception_translator);
    // Create the Python type object for our extension class and define __init__ function.
    class_<Gfal::GfalFile, boost::shared_ptr<Gfal::GfalFile> >("file",  init<const std::string &, const std::string &>())
        .def("read", &Gfal::GfalFile::read)  // Add a regular member function.
        .def("write", &Gfal::GfalFile::write)  
        .def("lseek", &Gfal::GfalFile::lseek)
        
    ;


   /* def("set_parameter_string", Gfal::set_parameter_string);
    
    def("set_parameter_boolean", Gfal::set_parameter_bool);
    
    def("get_parameter_string", &Gfal::get_parameter_string);
    
    def("get_parameter_boolean", Gfal::get_parameter_bool);*/
    
    
   // def("setparameter", &Gfal::getparameter);

}


