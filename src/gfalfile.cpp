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

#include <gfal_api.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include <unistd.h>
#include "gfal_boost_include.hpp"
#include "GErrorWrapper.h"


#include "gfalfile.h"

static const ssize_t MAX_BUFFER_SIZE = 4096;


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
Gfal::GfalFile::GfalFile(const Gfal & context,
                         const std::string & path,
                         const std::string & flag) :
    cont(context.getContext()),
    path(path),
    flag(flag)
{
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    fd = gfal2_open(cont->context, path.c_str(), convert_open_flag_py_to_cpp(flag), &tmp_err);
    if(fd <= 0)
        check_GError(&tmp_err);

}


/**
 * @brief destructor of a gfalFile object, close the file
 *
 **/
Gfal::GfalFile::~GfalFile()
{
    GfalPy::scopedGILRelease unlock;
    (void)gfal2_close(cont->context, fd, NULL);
}


std::string Gfal::GfalFile::read(size_t count) {
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    std::vector<char> buf(count + 1); // vector on the heap for massive buffer size
    ssize_t ret = gfal2_read(cont->context, fd, &(buf.front()), count, &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);

    buf[ret] ='\0';
    return std::string(&(buf.front()),ret);
}


std::string Gfal::GfalFile::pread(off_t offset, size_t count){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    std::vector<char> buf(count + 1); // vector on the heap for massive buffer size
    ssize_t ret = gfal2_pread(cont->context, fd, &(buf.front()), count, offset, &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);

    buf[ret] ='\0';
    return std::string(&(buf.front()),ret);
}

ssize_t Gfal::GfalFile::write(const std::string & str){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    const size_t s_str = str.size();

    ssize_t ret = gfal2_write(cont->context, fd, str.c_str(), s_str, &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);
    return ret;
}

ssize_t Gfal::GfalFile::pwrite(const std::string & str, off_t offset) {
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    const size_t s_str = str.size();

    ssize_t ret = gfal2_pwrite(cont->context, fd, str.c_str(), s_str, offset, &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);
    return ret;
}


off_t Gfal::GfalFile::lseek(off_t offset, int flag) {
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    off_t ret = gfal2_lseek(cont->context,fd, offset, flag, &tmp_err);
    if(ret ==  ((off_t)0)-1)
        check_GError(&tmp_err);
    return ret;
}

/**
 *********** GFAL DIRECTORY
 */

Gfal::GfalDirectory::GfalDirectory(const Gfal & context,
        const std::string & path) :
        cont(context.getContext()), path(path)
{
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err = NULL;
    d = gfal2_opendir(cont->context, path.c_str(), &tmp_err);
    if (d == NULL)
        check_GError(&tmp_err);
}

Gfal::GfalDirectory::~GfalDirectory()
{
    GfalPy::scopedGILRelease unlock;
    (void) gfal2_closedir(cont->context, d, NULL);
}

boost::python::tuple Gfal::GfalDirectory::readpp()
{
    GError* tmp_err = NULL;
    boost::shared_ptr<Gfal::GDirent> dirent;
    boost::shared_ptr<Gfal::GStat> stat(new Gfal::GStat());

    {
        GfalPy::scopedGILRelease unlock;
        dirent.reset(new Gfal::GDirent(gfal2_readdirpp(cont->context, d, &stat->_st, &tmp_err)));
    }
    if (dirent->isValid() == false) {
        check_GError(&tmp_err);
        return boost::python::make_tuple(boost::python::object(), boost::python::object());
    }

    return boost::python::make_tuple(dirent, stat);
}

boost::shared_ptr<Gfal::GDirent> Gfal::GfalDirectory::read() {
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;

    boost::shared_ptr<Gfal::GDirent> dirent(new Gfal::GDirent(gfal2_readdir(cont->context, d, &tmp_err)));
    check_GError(&tmp_err);
    return dirent;
}




/*******************
 *
 */

int Gfal::filecopy(const std::string &src, const std::string &dst) {
    GfalPy::scopedGILRelease unlock;
    GError * tmp_err=NULL;
    int ret = gfalt_copy_file(cont->context, NULL, src.c_str(), dst.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return ret;
}


int Gfal::filecopy(const Gfalt_params & p, const std::string & src, const std::string & dst) {
    GfalPy::scopedGILRelease unlock;
    GError * tmp_err=NULL;
    int ret = gfalt_copy_file(cont->context, p.params, src.c_str(), dst.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return ret;
}


boost::python::object Gfal::filecopy(const boost::python::list& srcs,
        const boost::python::list& dsts)
{
    Gfalt_params params;
    return filecopy(params, srcs, dsts);
}

boost::python::object Gfal::filecopy(const Gfalt_params & p, const boost::python::list& srcs,
        const boost::python::list& dsts)
{
    boost::python::list no_chksums;
    return filecopy(p, srcs, dsts, no_chksums);
}

boost::python::object Gfal::filecopy(const Gfalt_params & p, const boost::python::list& srcs,
        const boost::python::list& dsts,
        const boost::python::list& chksums)
{
    GError * op_error = NULL;
    GError** file_errors = NULL;

    long nbfiles = boost::python::len(srcs);
    long nbchksum = boost::python::len(chksums);

    if (nbfiles != boost::python::len(dsts))
        throw GErrorWrapper("Number of sources and destinations do not match", EINVAL);
    if (nbfiles != nbchksum && nbchksum != 0)
        throw GErrorWrapper("Number of pairs and checksums do not match", EINVAL);

    std::vector<std::string> sources;
    std::vector<std::string> destinations;
    std::vector<std::string> checksums;
    const char* sources_ptr[nbfiles];
    const char* destinations_ptr[nbfiles];
    const char* checksums_ptr[nbfiles];
    int ret;

    for (long i = 0; i < nbfiles; ++i) {
        sources.push_back(boost::python::extract<std::string>(srcs[i]));
        destinations.push_back(boost::python::extract<std::string>(dsts[i]));
        sources_ptr[i] = sources.back().c_str();
        destinations_ptr[i] = destinations.back().c_str();
        if (nbchksum) {
            checksums.push_back(boost::python::extract<std::string>(chksums[i]));
            checksums_ptr[i] = checksums.back().c_str();
        }
    }

    {
        GfalPy::scopedGILRelease unlock;

        if (nbchksum == 0) {
            ret = gfalt_copy_bulk(cont->context, p.params, nbfiles, sources_ptr,
                    destinations_ptr, NULL, &op_error, &file_errors);
        }
        else {
            ret = gfalt_copy_bulk(cont->context, p.params, nbfiles, sources_ptr,
                    destinations_ptr, checksums_ptr, &op_error, &file_errors);
        }

        if (ret < 0 && op_error != NULL)
            check_GError(&op_error);
    }

    boost::python::list pyerrors;
    GError2PyError(pyerrors, nbfiles, file_errors);
    g_free(file_errors);
    return pyerrors;
}

/**
 * wrapper to gfal_lstat function
 */
Gfal::GStat Gfal::lstat(const std::string & path) {
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    GStat st;
    const int ret = gfal2_lstat(cont->context, path.c_str(), &st._st, &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);
    return st;
}

/**
 * wrapper to gfal_stat function
 */
Gfal::GStat Gfal::stat_c(const std::string & path) {
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    GStat st;
    const int ret = gfal2_stat(cont->context, path.c_str(), &st._st, &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);
    return st;
}

/**
 * wrapper to gfal_access function
 *
 * */
int Gfal::access(const std::string & path, int flag){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    const int ret = gfal2_access(cont->context, path.c_str(), flag, &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);
    return ret;
}

/**
 * wrapper to gfal_chmod function
 *
 * */
int Gfal::chmod(const std::string & path, mode_t mode){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    const int ret = gfal2_chmod(cont->context, path.c_str(), mode, &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);
    return 0;
}


/**
 * wrapper to gfal_unlink function
 *
 * */
int Gfal::unlink(const std::string & path){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    const int ret = gfal2_unlink(cont->context, path.c_str(), &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);
    return 0;
}


/**
 * wrapper to gfal_mkdir function
 *
 * */
int Gfal::mkdir(const std::string & path, mode_t mode){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    const int ret = gfal2_mkdir(cont->context, path.c_str(), mode, &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);
    return 0;
}

/**
 * wrapper to gfal_mkdir_rec function
 *
 * */
int Gfal::mkdir_rec(const std::string & path, mode_t mode){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    const int ret = gfal2_mkdir_rec(cont->context, path.c_str(), mode, &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);
    return 0;
}

/**
 * wrapper to gfal_mkdir function
 *
 * */
int Gfal::rmdir(const std::string & path){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    const int ret = gfal2_rmdir(cont->context, path.c_str(), &tmp_err);
    if(ret <  0)
        check_GError(&tmp_err);
    return 0;
}

/**
 * wrapper to a concat gfal_opendir/readdir/closedir
 *
 * */
boost::python::list Gfal::listdir(const std::string & path){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    DIR* d = gfal2_opendir(cont->context, path.c_str(), &tmp_err);
    if(d== NULL)
        check_GError(&tmp_err);
    boost::python::list resu;
    struct dirent* st;
    while( (st= gfal2_readdir(cont->context, d, &tmp_err) ) != NULL){
        resu.append<std::string>(std::string(st->d_name));
    }
    GError* close_error = NULL;
    gfal2_closedir(cont->context, d, &close_error);
    check_GError(&tmp_err);
    check_GError(&close_error);

    return resu;
}

/**
 * wrapper to the gfal_rename func
 **/
int Gfal::rename(const std::string & src, const std::string & dest){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    int ret = gfal2_rename(cont->context, src.c_str(), dest.c_str(), &tmp_err);
    if(ret != 0)
        check_GError(&tmp_err);
    return 0;
}

/**
 * wrapper to the gfal_readlink func
 **/
std::string Gfal::readlink(const std::string & path){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    char buffer[MAX_BUFFER_SIZE];
    ssize_t ret = gfal2_readlink(cont->context, path.c_str(), buffer, MAX_BUFFER_SIZE, &tmp_err);
    if(ret < 0)
        check_GError(&tmp_err);
    return buffer;
}


std::string Gfal::checksum(const std::string & uri, const std::string & chk_type,
                 off_t start_offset, size_t data_length){
    char buffer[MAX_BUFFER_SIZE];
    GError* tmp_err=NULL;
    gfal2_checksum(cont->context, uri.c_str(), chk_type.c_str(), start_offset, data_length,
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
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    int ret = gfal2_symlink(cont->context, oldpath.c_str(), newpath.c_str(), &tmp_err);
    if(ret != 0)
        check_GError(&tmp_err);
    return 0;
}

/**
 * wrapper to the gfal get extended attributes
 *
 */
std::string Gfal::getxattr(const std::string & file, const std::string & key){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    char buffer[MAX_BUFFER_SIZE];
    const ssize_t ret = gfal2_getxattr(cont->context, file.c_str(), key.c_str(), buffer, MAX_BUFFER_SIZE, &tmp_err);
    if( ret < 0)
        check_GError(&tmp_err);
    return std::string(buffer);
}

/**
 * wrapper to the gfal set extended attributes
 *
 */
int Gfal::setxattr(const std::string  & file, const std::string & key, const std::string & value, int flag){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    const ssize_t ret = gfal2_setxattr(cont->context, file.c_str(), key.c_str(), value.c_str(), value.size()+1, flag, &tmp_err);
    if( ret < 0)
        check_GError(&tmp_err);
    return 0;
}

/**
 * wrapper to the gfal list extended attributes
 *
 */
boost::python::list Gfal::listxattr(const std::string & file ){
    GfalPy::scopedGILRelease unlock;
    GError* tmp_err=NULL;
    char buffer[MAX_BUFFER_SIZE];
    const ssize_t ret = gfal2_listxattr(cont->context, file.c_str(), buffer, MAX_BUFFER_SIZE, &tmp_err);
    if( ret < 0)
        check_GError(&tmp_err);

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
 * wrapper for bring online and family
 */
boost::python::tuple Gfal::bring_online(const std::string& path, time_t pintime,
                         time_t timeout, bool async)
{
    GError* tmp_err = NULL;
    char token[128] = {0};
    int ret = gfal2_bring_online(cont->context, path.c_str(),
                                 pintime, timeout,
                                 token, sizeof(token),
                                 async, &tmp_err);

    if (ret < 0)
        check_GError(&tmp_err);

    return boost::python::make_tuple(ret, std::string(token));
}

int Gfal::bring_online_poll(const std::string& path, const std::string& token)
{
    GError* tmp_err = NULL;
    int ret = gfal2_bring_online_poll(cont->context, path.c_str(), token.c_str(), &tmp_err);
    if (ret < 0)
        check_GError(&tmp_err);
    return ret;
}


int Gfal::release(const std::string& path, const std::string& token)
{
    GError* tmp_err = NULL;
    int ret = gfal2_release_file(cont->context, path.c_str(), token.c_str(), &tmp_err);
    if (ret < 0)
        check_GError(&tmp_err);
    return ret;
}

boost::python::tuple Gfal::bring_online_list(const boost::python::list& pyfiles, time_t pintime,
        time_t timeout, bool async)
{
    size_t nbfiles = boost::python::len(pyfiles);
    if (nbfiles == 0)
        throw GErrorWrapper("Empty list of files", EINVAL);

    std::vector<std::string> files(nbfiles);
    std::vector<GError*> errors(nbfiles, NULL);
    const char* files_ptr[nbfiles];

    for (size_t i = 0; i < nbfiles; ++i) {
        files.push_back(boost::python::extract<std::string>(pyfiles[i]));
        files_ptr[i] = files.back().c_str();
    }

    char token[128] = {0};
    gfal2_bring_online_list(cont->context, nbfiles, files_ptr,
                    pintime, timeout, token, sizeof(token), async, errors.data());

    boost::python::list pyerrors;
    GError2PyError(pyerrors, nbfiles, errors.data());
    return boost::python::make_tuple(pyerrors, std::string(token));
}


boost::python::list Gfal::bring_online_poll_list(const boost::python::list& pyfiles, const std::string& token)
{
    size_t nbfiles = boost::python::len(pyfiles);
    if (nbfiles == 0)
        throw GErrorWrapper("Empty list of files", EINVAL);

    std::vector<std::string> files(nbfiles);
    std::vector<GError*> errors(nbfiles, NULL);
    const char* files_ptr[nbfiles];

    for (size_t i = 0; i < nbfiles; ++i) {
        files.push_back(boost::python::extract<std::string>(pyfiles[i]));
        files_ptr[i] = files.back().c_str();
    }

    gfal2_bring_online_poll_list(cont->context, nbfiles, files_ptr, token.c_str(), errors.data());

    boost::python::list pyerrors;
    GError2PyError(pyerrors, nbfiles, errors.data());
    return pyerrors;
}


boost::python::list Gfal::release_list(const boost::python::list& pyfiles, const std::string& token)
{
    size_t nbfiles = boost::python::len(pyfiles);
    if (nbfiles == 0)
        throw GErrorWrapper("Empty list of files", EINVAL);

    std::vector<std::string> files(nbfiles);
    std::vector<GError*> errors(nbfiles, NULL);
    const char* files_ptr[nbfiles];

    for (size_t i = 0; i < nbfiles; ++i) {
        files.push_back(boost::python::extract<std::string>(pyfiles[i]));
        files_ptr[i] = files.back().c_str();
    }

    gfal2_release_file_list(cont->context, nbfiles, files_ptr, token.c_str(), errors.data());

    boost::python::list pyerrors;
    GError2PyError(pyerrors, nbfiles, errors.data());
    return pyerrors;
}


/**
 * Gfal file open
 * */
boost::shared_ptr<Gfal::GfalFile> Gfal::open(const std::string & path, const std::string &flag){
    return boost::shared_ptr<Gfal::GfalFile>(new Gfal::GfalFile(*this, path, flag));
}

boost::shared_ptr<Gfal::GfalFile> Gfal::file(const std::string & path, const std::string &flag){
    return open(path, flag);
}

boost::shared_ptr<Gfal::GfalDirectory> Gfal::opendir(const std::string & path) {
    return boost::shared_ptr<Gfal::GfalDirectory>(new Gfal::GfalDirectory(*this, path));
}

boost::shared_ptr<Gfal::GfalDirectory> Gfal::directory(const std::string & path) {
    return opendir(path);
}

int Gfal::get_opt_integer(const std::string & nmspace, const std::string & key){
    GError * tmp_err=NULL;
     int ret = gfal2_get_opt_integer(cont->context, nmspace.c_str(), key.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

std::string Gfal::get_opt_string(const std::string & nmspace, const std::string & key){
    GError * tmp_err=NULL;
    char* p = gfal2_get_opt_string(cont->context, nmspace.c_str(), key.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return std::string(p);
}

boost::python::list Gfal::get_opt_string_list(const std::string & nmspace, const std::string & key){
    GError * tmp_err=NULL;
    gsize size=0;
    boost::python::list result;
    char** res =  gfal2_get_opt_string_list(cont->context, nmspace.c_str(), key.c_str(), &size, &tmp_err);
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
    const bool ret = gfal2_get_opt_boolean(cont->context, nmspace.c_str(), key.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

int Gfal::set_opt_integer(const std::string & nmspace, const std::string & key, int value){
    GError * tmp_err=NULL;
    int ret = gfal2_set_opt_integer(cont->context, nmspace.c_str(), key.c_str(), value, &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

int Gfal::set_opt_string(const std::string & nmspace, const std::string & key, const std::string & value){
    GError * tmp_err=NULL;
    int ret = gfal2_set_opt_string(cont->context, nmspace.c_str(), key.c_str(), (char*)value.c_str(), &tmp_err);
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

    int ret = gfal2_set_opt_string_list(cont->context, nmspace.c_str(), key.c_str(), tab_ptr, size_list, &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

int Gfal::set_opt_boolean(const std::string & nmspace, const std::string & key, bool val){
    GError * tmp_err=NULL;
    int ret = gfal2_set_opt_boolean(cont->context, nmspace.c_str(), key.c_str(), val, &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

int Gfal::load_opts_from_file(const std::string & path) {
    GError * tmp_err=NULL;
    int ret = gfal2_load_opts_from_file(cont->context, path.c_str(), &tmp_err);
    check_GError(&tmp_err);
    return ret;
}

boost::python::list Gfal::get_plugin_names(void) {
    boost::python::list pyplugins;
    gchar** plugins = gfal2_get_plugin_names(cont->context);
    int nplugins = g_strv_length(plugins);

    for (int i = 0; i < nplugins; ++i) {
        pyplugins.append(std::string(plugins[i]));
    }
    g_strfreev(plugins);

    return pyplugins;
}

boost::shared_ptr<Gfal> create_instance(){
    return boost::shared_ptr<Gfal>(new Gfal());
}

int gfal_set_verbose_enum(gfal_verbose_levels lvls)
{
    gfal_set_verbose((int) lvls);
    return 0;
}

int Gfal::cancel(){
    GfalPy::scopedGILRelease unlock;
    return gfal2_cancel(cont->context);
}

