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
 



#include <gfal_api.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include <unistd.h>
#include "gfal_boost_include.hpp"

#include "gfalfile.h"

static const ssize_t MAX_BUFFER_SIZE=4096;


static int convert_open_flag_py_to_cpp(const std::string & str){
	if( str.compare("rw") == 0 || str.compare("rw") == 0)
		return O_RDWR | O_CREAT;
	if( str.compare("r") == 0)
		return O_RDONLY ;
	if( str.compare("w") == 0)
        return O_WRONLY | O_CREAT | O_TRUNC;
	throw std::runtime_error("Invalid open flag, must be r, w, or rw");
}

/**
 * @brief main constructor to open a file
 * @param path : path to the file to open
 * @param flag : flag of the file
 **/
Gfal::GfalFile::GfalFile(const std::string & path, const std::string & flag) : path(path), flag(flag) 
{
	Gfal_scopedGILRelease unlock;
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
	Gfal_scopedGILRelease unlock;
    (void)gfal_close(fd);
}


std::string Gfal::GfalFile::read(size_t count) {
	Gfal_scopedGILRelease unlock;
	
	std::auto_ptr< std::vector<char> > buf(new std::vector<char>(count+1)); // vector on the heap for massive buffer size
	ssize_t ret = gfal_read(fd, &(buf->front()), count);
	if(ret <  0)
		gfal_GError_to_exception();
	
	(*buf)[ret] ='\0';
	return std::string(&(buf->front()),ret);
}


std::string Gfal::GfalFile::pread(off_t offset, size_t count){
    Gfal_scopedGILRelease unlock;

    std::auto_ptr< std::vector<char> > buf(new std::vector<char>(count+1)); // vector on the heap for massive buffer size
    ssize_t ret = gfal_pread(fd, &(buf->front()), count, offset);
    if(ret <  0)
        gfal_GError_to_exception();

    (*buf)[ret] ='\0';
    return std::string(&(buf->front()),ret);
}

ssize_t Gfal::GfalFile::write(const std::string & str){
	Gfal_scopedGILRelease unlock;
	
	const size_t s_str = str.size();

	ssize_t ret = gfal_write(fd, str.c_str(), s_str);
	if(ret <  0)
		gfal_GError_to_exception();	
	return ret;
}

ssize_t Gfal::GfalFile::pwrite(const std::string & str, off_t offset){
    Gfal_scopedGILRelease unlock;

    const size_t s_str = str.size();

    ssize_t ret = gfal_pwrite(fd, str.c_str(), s_str, offset);
    if(ret <  0)
        gfal_GError_to_exception();
    return ret;
}


off_t Gfal::GfalFile::lseek(off_t offset, int flag){
	Gfal_scopedGILRelease unlock;
	
	off_t ret = gfal_lseek(fd, offset, flag);
	if(ret ==  ((off_t)0)-1)
		gfal_GError_to_exception();	
	return ret;
}

int Gfal::filecopy(const std::string &src, const std::string &dst){
    GError * tmp_err=NULL;
    int ret =gfalt_copy_file(cont, NULL, src.c_str(), dst.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return ret;
}


int Gfal::filecopy(const Gfalt_params & p, const std::string & src, const std::string & dst){
    GError * tmp_err=NULL;
    int ret = gfalt_copy_file(cont, p.params, src.c_str(), dst.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

/**
 * wrapper to gfal_lstat function
 */
Gfal::Gstat Gfal::lstat(const std::string & path) {
	Gfal_scopedGILRelease unlock;
	
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
	Gfal_scopedGILRelease unlock;

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
	Gfal_scopedGILRelease unlock;
	
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
	Gfal_scopedGILRelease unlock;
	
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
	Gfal_scopedGILRelease unlock;
	
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
	Gfal_scopedGILRelease unlock;
	
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
	Gfal_scopedGILRelease unlock;
	
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
	Gfal_scopedGILRelease unlock;
	
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
	Gfal_scopedGILRelease unlock;
	
	int ret = gfal_rename(src.c_str(), dest.c_str());
	if(ret != 0)
		gfal_GError_to_exception();	
	return 0;
}

/**
 * wrapper to the gfal_readlink func
 **/
std::string Gfal::readlink(const std::string & path){
	Gfal_scopedGILRelease unlock;
		
	char buffer[MAX_BUFFER_SIZE];
	ssize_t ret = gfal_readlink(path.c_str(), buffer, MAX_BUFFER_SIZE);
	if(ret < 0)
		gfal_GError_to_exception();	
	return buffer;		
}


std::string Gfal::checksum(const std::string & uri, const std::string & chk_type,
                 off_t start_offset, size_t data_length){
    char buffer[MAX_BUFFER_SIZE];
    GError* tmp_err=NULL;
    gfal2_checksum(cont, uri.c_str(), chk_type.c_str(), start_offset, data_length,
                                    buffer, MAX_BUFFER_SIZE, &tmp_err);
    check_GError(&tmp_err);
    return buffer;
}

std::string Gfal::checksum(const std::string & uri, const std::string & chk_type){
    return checksum(uri, chk_type, 0,0);
}

/**
 * wrapper to the gfal_symlink func
 **/
int Gfal::symlink(const std::string & oldpath, const std::string & newpath){
	Gfal_scopedGILRelease unlock;
	
	int ret = gfal_symlink(oldpath.c_str(), newpath.c_str());
	if(ret != 0)
		gfal_GError_to_exception();	
	return 0;	
}

/**
 * wrapper to the gfal get extended attributes
 * 
 */
std::string Gfal::getxattr(const std::string & file, const std::string & key){
	Gfal_scopedGILRelease unlock;	
	
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
int Gfal::setxattr(const std::string  & file, const std::string & key, const std::string & value, int flag){
	Gfal_scopedGILRelease unlock;	
	
	const ssize_t ret = gfal_setxattr(file.c_str(), key.c_str(), value.c_str(), value.size()+1, flag);	
	if( ret < 0)
		gfal_GError_to_exception();	
	return 0;		
}

/**
 * wrapper to the gfal list extended attributes
 * 
 */
boost::python::list Gfal::listxattr(const std::string & file ){
	Gfal_scopedGILRelease unlock;	
	
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



/**
 * Gfal file open
 * */
boost::shared_ptr<Gfal::GfalFile> Gfal::open(const std::string & path, const std::string &flag){
	return boost::shared_ptr<Gfal::GfalFile>(new Gfal::GfalFile(path, flag));
}

int Gfal::get_opt_integer(const std::string & nmspace, const std::string & key){
    GError * tmp_err=NULL;
     int ret = gfal2_get_opt_integer(cont, nmspace.c_str(), key.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

std::string Gfal::get_opt_string(const std::string & nmspace, const std::string & key){
    GError * tmp_err=NULL;
    char* p = gfal2_get_opt_string(cont, nmspace.c_str(), key.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return std::string(p);
}

boost::python::list Gfal::get_opt_string_list(const std::string & nmspace, const std::string & key){
    GError * tmp_err=NULL;
    gsize size=0;
    boost::python::list result;
    char** res =  gfal2_get_opt_string_list(cont, nmspace.c_str(), key.c_str(), &size, &tmp_err);
    check_GError(&tmp_err);
    if(res){
        for(size_t i =0; i < size; i++)
            result.append(std::string(res[i]));
        g_strfreev(res);
    }
    return result;
}

bool Gfal::get_opt_boolean(const std::string & nmspace, const std::string & key){
    GError * tmp_err=NULL;
    const bool ret = gfal2_get_opt_boolean(cont, nmspace.c_str(), key.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

int Gfal::set_opt_integer(const std::string & nmspace, const std::string & key, int value){
    GError * tmp_err=NULL;
    int ret = gfal2_set_opt_integer(cont, nmspace.c_str(), key.c_str(), value, &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

int Gfal::set_opt_string(const std::string & nmspace, const std::string & key, const std::string & value){
    GError * tmp_err=NULL;
    int ret = gfal2_set_opt_string(cont, nmspace.c_str(), key.c_str(), (char*)value.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

int Gfal::set_opt_string_list(const std::string & nmspace, const std::string & key, const boost::python::list  & py_value){
    std::vector<std::string>  value = convert_python_list_to_typed_list<std::string>(py_value);

    GError * tmp_err=NULL;
    const int size_list = value.size();
    char* tab_ptr[size_list+1];
    for(int i = 0; i < size_list; i++){
        tab_ptr[i] = (char*) value[i].c_str();
    }
    tab_ptr[size_list] = NULL;

    int ret = gfal2_set_opt_string_list(cont, nmspace.c_str(), key.c_str(), tab_ptr, size_list, &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

int Gfal::set_opt_boolean(const std::string & nmspace, const std::string & key, bool val){
    GError * tmp_err=NULL;
    int ret = gfal2_set_opt_boolean(cont, nmspace.c_str(), key.c_str(), val, &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

boost::shared_ptr<Gfal> create_instance(){
    return boost::shared_ptr<Gfal>(new Gfal());
}

int gfal_set_verbose_enum(gfal_verbose_levels lvls)
{
    gfal_set_verbose((int) lvls);
    return 0;
}



