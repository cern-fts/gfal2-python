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
#include <vector>
#include <boost/python.hpp>
#include <cstring>
#include <unistd.h>

#include "gerror_exception.h"
#include "gfalcpp.h"

#include "gfalfile.h"

static const ssize_t MAX_BUFFER_SIZE=4096;


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
	const int ret = gfal_close(fd);
}


std::string Gfal::GfalFile::read(size_t count) {
	std::auto_ptr< std::vector<char> > buf(new std::vector<char>(count+1)); // vector on the heap for massive buffer size
	ssize_t ret = gfal_read(fd, &(buf->front()), count);
	if(ret <  0)
		gfal_GError_to_exception();
	
	(*buf)[ret] ='\0';
	return std::string(&(buf->front()),ret);
}


ssize_t Gfal::GfalFile::write(const std::string & str){
	const size_t s_str = str.size();

	ssize_t ret = gfal_write(fd, str.c_str(), s_str);
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

/**
 * wrapper to a concat gfal_opendir/readdir/closedir 
 * 
 * */
boost::python::list Gfal::listdir(const std::string & path){
	DIR* d = gfal_opendir(path.c_str());
	if(d== NULL)
		gfal_GError_to_exception();	
	boost::python::list resu;
	struct dirent* st;
	while( (st= gfal_readdir(d) ) != NULL){
		resu.append<std::string>(std::string(st->d_name));
	}
	gfal_closedir(d);
	if(gfal_posix_code_error())
		gfal_GError_to_exception();		
	
	return resu;
}

/**
 * wrapper to the gfal_rename func
 **/
int Gfal::rename(const std::string & src, const std::string & dest){
	int ret = gfal_rename(src.c_str(), dest.c_str());
	if(ret != 0)
		gfal_GError_to_exception();	
	return 0;
}

/**
 * wrapper to the gfal_readlink func
 **/
std::string Gfal::readlink(const std::string & path){
	char buffer[MAX_BUFFER_SIZE];
	ssize_t ret = gfal_readlink(path.c_str(), buffer, MAX_BUFFER_SIZE);
	if(ret < 0)
		gfal_GError_to_exception();	
	return buffer;		
}

/**
 * wrapper to the gfal_symlink func
 **/
int Gfal::symlink(const std::string & oldpath, const std::string & newpath){
	int ret = gfal_symlink(oldpath.c_str(), newpath.c_str());
	if(ret != 0)
		gfal_GError_to_exception();	
	return 0;	
}

/**
 * wrapper to the gfal get extended attributes
 * 
 */
std::string Gfal::getxattr(std::string file, std::string key){
	char buffer[MAX_BUFFER_SIZE];
	const ssize_t ret = gfal_getxattr(file.c_str(), key.c_str(), buffer, MAX_BUFFER_SIZE);
	if( ret < 0)
		gfal_GError_to_exception();	
	return std::string(buffer);	 
}

/**
 * wrapper to the gfal set extended attributes
 * 
 */
int Gfal::setxattr(std::string file, std::string key, std::string value, int flag){
	const ssize_t ret = gfal_setxattr(file.c_str(), key.c_str(), value.c_str(), value.size()+1, flag);	
	if( ret < 0)
		gfal_GError_to_exception();	
	return 0;		
}

/**
 * wrapper to the gfal list extended attributes
 * 
 */
boost::python::list Gfal::listxattr(std::string file ){
	char buffer[MAX_BUFFER_SIZE];
	const ssize_t ret = gfal_listxattr(file.c_str(), buffer, MAX_BUFFER_SIZE);	
	if( ret < 0)
		gfal_GError_to_exception();	
		
	boost::python::list resu;		
	ssize_t current=0;
	while(current < ret){
		std::string tmp(buffer+current);
		resu.append(tmp);
		current += tmp.size()+1;	
	}
	return resu;		
}
