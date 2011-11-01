//      gfalfile.cpp
//      
//      Copyright 2011 didi <didi@didi-laptop>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

/**
 * main file for boost python wrapper
 * 
 * */
 
#include <boost/python.hpp>
#include <iostream>
#include <string>
#include <limits.h>

#include "gfalfile.h"
#include "gfal_stat.h"
#include "gfalcpp.h"

#if PYTHON_VERSION_MAJOR == 2 && PYTHON_VERSION_MINOR == 6 && ( __WORDSIZE == 64 )
#include <python2.6/modsupport.h> // correct 2.6 include bug

#endif

using namespace boost::python;

void gerror_exception_translator(const Gerror_exception  & x){
    PyErr_SetString(PyExc_RuntimeError, x.what());
}



BOOST_PYTHON_MODULE(gfal2)
{
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
    class_<Gfal::GfalFile>("file", init<const std::string &, const std::string &>())
        .def("read", &Gfal::GfalFile::read)  // Add a regular member function.
        .def("write", &Gfal::GfalFile::write)  
        .def("lseek", &Gfal::GfalFile::lseek)
        
    ;
    
    def("access", &Gfal::access);
    
    def("lstat", &Gfal::lstat);
    
    def("stat", &Gfal::stat_c);
    
    def("chmod", &Gfal::chmod);
    
    def("unlink", &Gfal::unlink);
    
    def("mkdir", &Gfal::mkdir);
    
    def("rmdir", &Gfal::rmdir);
    
    def("listdir", &Gfal::listdir);

}


