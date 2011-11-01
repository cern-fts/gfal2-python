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



#include <gfal_api.h>
#include <algorithm>
#include <boost/python.hpp>
#include <cstring>
#include <unistd.h>

#include "gerror_exception.h"
#include "gfalcpp.h"

#include "gfalfile.h"

static const ssize_t MAX_BUFFER_SIZE=2048;


static int convert_open_flag_py_to_cpp(const std::string & str){
	if( str.compare("rw") == 0 || str.compare("rw") == 0)
		return O_RDWR | O_CREAT;
	if( str.compare("r") == 0)
		return O_RDONLY ;
	if( str.compare("w") == 0)
		return O_WRONLY | O_CREAT;
	throw std::runtime_error("Invalid open flag, must be r, w, or rw");
}

/**
 * @brief main constructor to open a file
 * @param path : path to the file to open
 * @param flag : flag of the file
 **/
Gfal::GfalFile::GfalFile(const std::string & path, const std::string & flag) : path(path), flag(flag) 
{

	fd = gfal_open(path.c_str(), convert_open_flag_py_to_cpp(flag) );
	if(fd <= 0)
		gfal_GError_to_exception();

}

/**
 * @brief destructor of a gfalFile object, close the file
 * 
 **/
Gfal::GfalFile::~GfalFile() 
{

	int ret = gfal_close(fd);
}


std::string Gfal::GfalFile::read(size_t count) {
	char buf[count+1];
	ssize_t ret = gfal_read(fd, buf, count);
	if(ret <  0)
		gfal_GError_to_exception();
	buf[ std::min<ssize_t>(ret, count) ] ='\0';
	return std::string(buf);
}


ssize_t Gfal::GfalFile::write(const std::string & str){
	const size_t s_str = str.size();
	char buf[s_str +1];
	
	*((char*) mempcpy(buf,str.c_str(), s_str)) = '\0';
	ssize_t ret = gfal_write(fd, buf, s_str);
	if(ret <  0)
		gfal_GError_to_exception();	
	return ret;
}

off_t Gfal::GfalFile::lseek(off_t offset, int flag){
	
	off_t ret = gfal_lseek(fd, offset, flag);
	if(ret ==  ((off_t)0)-1)
		gfal_GError_to_exception();	
	return ret;
}


/**
 * wrapper to gfal_lstat function
 */
Gfal::Gstat Gfal::lstat(const std::string & path) {
	Gstat st;
	const int ret = gfal_lstat(path.c_str(), &st);
	if(ret <  0)
		gfal_GError_to_exception();	
	return st;	
}

/**
 * wrapper to gfal_stat function
 */
Gfal::Gstat Gfal::stat_c(const std::string & path) {
	Gstat st;
	const int ret = gfal_stat(path.c_str(), &st);
	if(ret <  0)
		gfal_GError_to_exception();	
	return st;	
}

/**
 * wrapper to gfal_access function
 * 
 * */
int Gfal::access(const std::string & path, int flag){
	const int ret = gfal_access(path.c_str(), flag);
	if(ret <  0)
		gfal_GError_to_exception();	
	return ret;
}

/**
 * wrapper to gfal_chmod function
 * 
 * */
int Gfal::chmod(const std::string & path, mode_t mode){
	const int ret = gfal_chmod(path.c_str(), mode);
	if(ret <  0)
		gfal_GError_to_exception();	
	return 0;
}


/**
 * wrapper to gfal_unlink function
 * 
 * */
int Gfal::unlink(const std::string & path){
	const int ret = gfal_unlink(path.c_str());
	if(ret <  0)
		gfal_GError_to_exception();	
	return 0;	
}


/**
 * wrapper to gfal_mkdir function
 * 
 * */
int Gfal::mkdir(const std::string & path, mode_t mode){
	const int ret = gfal_mkdir(path.c_str(), mode);
	if(ret <  0)
		gfal_GError_to_exception();	
	return 0;	
}

/**
 * wrapper to gfal_mkdir function
 * 
 * */
int Gfal::rmdir(const std::string & path){
	const int ret = gfal_rmdir(path.c_str());
	if(ret <  0)
		gfal_GError_to_exception();	
	return 0;	
}



