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

#include <algorithm>
#include <boost/python.hpp>
#include <list>
#include <vector>

#include <gfal_api.h>

#include "GErrorWrapper.h"
#include "Gfal2Context.h"
#include "pyGIL.h"

using namespace PyGfal2;

static const ssize_t MAX_BUFFER_SIZE = 4096;


// extract typed array from python array, throw type error in case of bad conversion
template<typename T>
static std::vector<T> convert_python_list_to_typed_list(
        const boost::python::list & l)
{
    std::vector<T> res;
    for (int i = 0; i < boost::python::len(l); ++i) {
        res.push_back(boost::python::extract<T>(l[i]));
    }
    return res;
}


int Gfal2Context::filecopy(const std::string &src, const std::string &dst)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    int ret = gfalt_copy_file(cont->get(), NULL, src.c_str(), dst.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


int Gfal2Context::filecopy(const GfaltParams & p, const std::string & src,
        const std::string & dst)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    int ret = gfalt_copy_file(cont->get(), p.params, src.c_str(), dst.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


boost::python::object Gfal2Context::filecopy(const boost::python::list& srcs,
        const boost::python::list& dsts)
{
    GfaltParams params;
    return filecopy(params, srcs, dsts);
}


boost::python::object Gfal2Context::filecopy(const GfaltParams & p, const boost::python::list& srcs,
        const boost::python::list& dsts)
{
    boost::python::list no_chksums;
    return filecopy(p, srcs, dsts, no_chksums);
}


boost::python::object Gfal2Context::filecopy(const GfaltParams & p, const boost::python::list& srcs,
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
        ScopedGILRelease unlock;

        if (nbchksum == 0) {
            ret = gfalt_copy_bulk(cont->get(), p.params, nbfiles, sources_ptr,
                    destinations_ptr, NULL, &op_error, &file_errors);
        }
        else {
            ret = gfalt_copy_bulk(cont->get(), p.params, nbfiles, sources_ptr,
                    destinations_ptr, checksums_ptr, &op_error, &file_errors);
        }

        if (ret < 0 && op_error != NULL)
            GErrorWrapper::throwOnError(&op_error);
    }

    boost::python::list pyerrors;
    GError2PyError(pyerrors, nbfiles, file_errors);
    g_free(file_errors);
    return pyerrors;
}


Stat Gfal2Context::lstat(const std::string & path)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    Stat st;
    const int ret = gfal2_lstat(cont->get(), path.c_str(), &st._st, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return st;
}


Stat Gfal2Context::stat_c(const std::string & path)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    Stat st;
    const int ret = gfal2_stat(cont->get(), path.c_str(), &st._st, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return st;
}


int Gfal2Context::access(const std::string & path, int flag)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    const int ret = gfal2_access(cont->get(), path.c_str(), flag, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


int Gfal2Context::chmod(const std::string & path, mode_t mode)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    const int ret = gfal2_chmod(cont->get(), path.c_str(), mode, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return 0;
}


int Gfal2Context::unlink(const std::string & path)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    const int ret = gfal2_unlink(cont->get(), path.c_str(), &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return 0;
}


boost::python::list Gfal2Context::unlink_list(const boost::python::list& pyfiles)
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

    {
        ScopedGILRelease unlock;
        gfal2_unlink_list(cont->get(), nbfiles, files_ptr, errors.data());
    }

    boost::python::list pyerrors;
    GError2PyError(pyerrors, nbfiles, errors.data());
    return pyerrors;
}


int Gfal2Context::mkdir(const std::string & path, mode_t mode)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    const int ret = gfal2_mkdir(cont->get(), path.c_str(), mode, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return 0;
}


int Gfal2Context::mkdir_rec(const std::string & path, mode_t mode)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    const int ret = gfal2_mkdir_rec(cont->get(), path.c_str(), mode,
            &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return 0;
}


int Gfal2Context::rmdir(const std::string & path)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    const int ret = gfal2_rmdir(cont->get(), path.c_str(), &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return 0;
}


boost::python::list Gfal2Context::listdir(const std::string & path)
{
    GError* tmp_err = NULL;
    DIR* d = NULL;
    {
        ScopedGILRelease unlock;
        d = gfal2_opendir(cont->get(), path.c_str(), &tmp_err);
    }
    if (d == NULL) {
        GErrorWrapper::throwOnError(&tmp_err);
    }

    // Do the listing outside Python scope
    // Creating Python objects even via boost require acquiring the GIL
    std::list<std::string> temporary;
    {
        ScopedGILRelease unlock;
        struct dirent *st;

        while ((st = gfal2_readdir(cont->get(), d, &tmp_err)) != NULL) {
            temporary.push_back(std::string(st->d_name));
        }
    }

    GError* close_error = NULL;
    {
        ScopedGILRelease unlock;
        gfal2_closedir(cont->get(), d, &close_error);
    }

    GErrorWrapper::throwOnError(&tmp_err);
    GErrorWrapper::throwOnError(&close_error);

    // Convert to a Python list
    boost::python::list result;
    for (std::list<std::string>::const_iterator i = temporary.begin(); i != temporary.end(); ++i) {
        result.append<std::string>(*i);
    }
    return result;
}


int Gfal2Context::rename(const std::string & src, const std::string & dest)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    int ret = gfal2_rename(cont->get(), src.c_str(), dest.c_str(), &tmp_err);
    if (ret != 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return 0;
}


std::string Gfal2Context::readlink(const std::string & path)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    char buffer[MAX_BUFFER_SIZE];
    ssize_t ret = gfal2_readlink(cont->get(), path.c_str(), buffer, MAX_BUFFER_SIZE, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return buffer;
}


std::string Gfal2Context::checksum(const std::string & uri,
        const std::string & chk_type, off_t start_offset, size_t data_length)
{
    ScopedGILRelease unlock;
    char buffer[MAX_BUFFER_SIZE];
    GError* tmp_err = NULL;
    gfal2_checksum(cont->get(), uri.c_str(), chk_type.c_str(), start_offset,
            data_length, buffer, MAX_BUFFER_SIZE, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return buffer;
}


std::string Gfal2Context::checksum(const std::string & uri,
        const std::string & chk_type)
{
    return checksum(uri, chk_type, 0, 0);
}


int Gfal2Context::symlink(const std::string & oldpath,
        const std::string & newpath)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    int ret = gfal2_symlink(cont->get(), oldpath.c_str(), newpath.c_str(), &tmp_err);
    if (ret != 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return 0;
}


std::string Gfal2Context::getxattr(const std::string & file, const std::string & key)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    char buffer[MAX_BUFFER_SIZE];
    const ssize_t ret = gfal2_getxattr(cont->get(), file.c_str(), key.c_str(), buffer,
            MAX_BUFFER_SIZE, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return std::string(buffer);
}


int Gfal2Context::setxattr(const std::string & file, const std::string & key,
        const std::string & value, int flag)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    const ssize_t ret = gfal2_setxattr(cont->get(), file.c_str(), key.c_str(),
            value.c_str(), value.size() + 1, flag, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return 0;
}


boost::python::list Gfal2Context::listxattr(const std::string & file)
{
    GError* tmp_err = NULL;
    char buffer[MAX_BUFFER_SIZE];
    ssize_t ret;
    {
        ScopedGILRelease unlock;
        ret = gfal2_listxattr(cont->get(), file.c_str(), buffer,
                MAX_BUFFER_SIZE, &tmp_err);
    }
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);

    boost::python::list resu;
    ssize_t current = 0;
    while (current < ret) {
        std::string tmp(buffer + current);
        resu.append(tmp);
        current += tmp.size() + 1;
    }
    return resu;
}


boost::python::tuple Gfal2Context::bring_online(const std::string& path, time_t pintime,
        time_t timeout, bool async)
{
    GError* tmp_err = NULL;
    char token[128] = { 0 };
    int ret;
    {
        ScopedGILRelease unlock;
        ret = gfal2_bring_online(cont->get(), path.c_str(), pintime, timeout, token,
                sizeof(token), async, &tmp_err);
    }

    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);

    return boost::python::make_tuple(ret, std::string(token));
}

boost::python::list Gfal2Context::qos_check_classes(const std::string& url, const std::string& type)
{
    GError* tmp_err = NULL;
    char buffer[MAX_BUFFER_SIZE];
    ssize_t ret;
    {
        ScopedGILRelease unlock;
        ret = gfal2_qos_check_classes(cont->get(), url.c_str(), type.c_str(),
                                      buffer, MAX_BUFFER_SIZE, &tmp_err);
    }
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);

    std::string classes(buffer);
    std::istringstream iss(classes);
    std::string classToken;
    boost::python::list qos_classes;
    while (std::getline(iss, classToken, ',')) {
        qos_classes.append(classToken);
    }

    return qos_classes;
}

std::string Gfal2Context::check_file_qos(const std::string& fileUrl)
{
    ScopedGILRelease unlock;

    GError* tmp_err = NULL;
    char buffer[MAX_BUFFER_SIZE];
    ssize_t ret = gfal2_check_file_qos(cont->get(), fileUrl.c_str(),
                                       buffer, MAX_BUFFER_SIZE, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);

    return std::string(buffer);
}

boost::python::list Gfal2Context::check_available_qos_transitions(const std::string& qosClassUrl)
{
    GError* tmp_err = NULL;
    boost::python::list qos_transitions;
    char buffer[MAX_BUFFER_SIZE];
    ssize_t ret;
    {
        ScopedGILRelease unlock;
        ret = gfal2_check_available_qos_transitions(cont->get(), qosClassUrl.c_str(),
                                                    buffer, MAX_BUFFER_SIZE, &tmp_err);
    }
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);

    std::string transitions(buffer);
    std::istringstream iss(transitions);
		std::string transitionToken;
		while (std::getline(iss, transitionToken, ',')) {
			qos_transitions.append(transitionToken);
		}

    return qos_transitions;
}

std::string Gfal2Context::check_target_qos(const std::string& fileUrl)
{
    ScopedGILRelease unlock;

    GError* tmp_err = NULL;
    char buffer[MAX_BUFFER_SIZE];
    ssize_t ret = gfal2_check_target_qos(cont->get(), fileUrl.c_str(),
                                         buffer, MAX_BUFFER_SIZE, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);

    return std::string(buffer);
}

int Gfal2Context::change_object_qos(const std::string& fileUrl, const std::string& newQosClass)
{
    ScopedGILRelease unlock;

    GError* tmp_err = NULL;
    int ret = gfal2_change_object_qos(cont->get(), fileUrl.c_str(), newQosClass.c_str(), &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);

    return ret;
}


std::string Gfal2Context::token_retrieve(const std::string& url, const std::string& issuer,
                                         unsigned validity, bool write_access)
{
    boost::python::list no_activities;
    return token_retrieve(url, issuer, validity, write_access, no_activities);
}


std::string Gfal2Context::token_retrieve(const std::string& url, const std::string& issuer,
                                         unsigned validity, const boost::python::list& pyactivities)
{
    size_t n_activities = boost::python::len(pyactivities);
    if (n_activities == 0)
        throw GErrorWrapper("Empty list of activities", EINVAL);

    return token_retrieve(url, issuer, validity, false, pyactivities);
}


std::string Gfal2Context::token_retrieve(const std::string& url, const std::string& issuer,
                                         unsigned validity, bool write_access,
                                         const boost::python::list& pyactivities)
{
    // Increase size to account for final NULL element
    size_t n_activities = boost::python::len(pyactivities) + 1;
    std::vector<std::string> activities(n_activities);
    const char* activities_ptr[n_activities];
    char buffer[MAX_BUFFER_SIZE];
    GError* tmp_err = NULL;

    for (size_t i = 0; i < n_activities - 1; i++) {
        activities.push_back(boost::python::extract<std::string>(pyactivities[i]));
        activities_ptr[i] = activities.back().c_str();
    }
    activities_ptr[n_activities - 1] = NULL;


    ScopedGILRelease unlock;
    ssize_t ret = gfal2_token_retrieve(cont->get(), url.c_str(), issuer.c_str(),
                                       write_access, validity, activities_ptr,
                                       buffer, MAX_BUFFER_SIZE, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);

    return std::string(buffer);
}


int Gfal2Context::bring_online_poll(const std::string& path, const std::string& token)
{
    ScopedGILRelease unlock;

    GError* tmp_err = NULL;
    int ret = gfal2_bring_online_poll(cont->get(), path.c_str(), token.c_str(),
            &tmp_err);
    if (ret < 0 && tmp_err->code == EAGAIN) {
        g_error_free(tmp_err);
        ret = 0;
    }
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


int Gfal2Context::release(const std::string& path, const std::string& token)
{
    ScopedGILRelease unlock;

    GError* tmp_err = NULL;
    int ret = gfal2_release_file(cont->get(), path.c_str(), token.c_str(), &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}

boost::python::tuple Gfal2Context::bring_online_list(const boost::python::list& pyfiles,
        time_t pintime, time_t timeout, bool async)
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

    char token[128] = { 0 };

    {
        ScopedGILRelease unlock;
        gfal2_bring_online_list(cont->get(), nbfiles, files_ptr, pintime, timeout, token,
                                sizeof(token), async, errors.data());
    }

    boost::python::list pyerrors;
    GError2PyError(pyerrors, nbfiles, errors.data());
    return boost::python::make_tuple(pyerrors, std::string(token));
}


boost::python::list Gfal2Context::bring_online_poll_list(const boost::python::list& pyfiles, const std::string& token)
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

    {
        ScopedGILRelease unlock;
        gfal2_bring_online_poll_list(cont->get(), nbfiles, files_ptr, token.c_str(),
                                     errors.data());
    }

    boost::python::list pyerrors;
    GError2PyError(pyerrors, nbfiles, errors.data());
    return pyerrors;
}


boost::python::list Gfal2Context::release_list(const boost::python::list& pyfiles,
        const std::string& token)
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

    {
        ScopedGILRelease unlock;
        gfal2_release_file_list(cont->get(), nbfiles, files_ptr, token.c_str(),
                                errors.data());
    }

    boost::python::list pyerrors;
    GError2PyError(pyerrors, nbfiles, errors.data());
    return pyerrors;
}


int Gfal2Context::abort_bring_online(const std::string &path, const std::string& token)
{
    GError* error = NULL;
    const char* file_ptr = path.c_str();
    int ret;
    ScopedGILRelease unlock;
    ret = gfal2_abort_files(cont->get(), 1, &file_ptr, token.c_str(), &error);
    if (ret < 0)
        GErrorWrapper::throwOnError(&error);
    return ret;
}


boost::python::list Gfal2Context::abort_bring_online_list(
        const boost::python::list& pyfiles, const std::string& token)
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

    {
        ScopedGILRelease unlock;
        gfal2_abort_files(cont->get(), nbfiles, files_ptr, token.c_str(),
                          errors.data());
    }

    boost::python::list pyerrors;
    GError2PyError(pyerrors, nbfiles, errors.data());
    return pyerrors;
}


boost::shared_ptr<File> Gfal2Context::open(const std::string & path, const std::string &flag)
{
    return boost::shared_ptr<File>(new File(*this, path, flag));
}


boost::shared_ptr<File> Gfal2Context::file(const std::string & path, const std::string &flag)
{
    return open(path, flag);
}


boost::shared_ptr<Directory> Gfal2Context::opendir(const std::string & path)
{
    return boost::shared_ptr<Directory>(new Directory(*this, path));
}


boost::shared_ptr<Directory> Gfal2Context::directory(const std::string & path)
{
    return opendir(path);
}

bool Gfal2Context::remove_opt(const std::string & group_name, const std::string & key)
{
   ScopedGILRelease unlock;
   GError * tmp_err = NULL;
   bool ret = gfal2_remove_opt(cont->get(), group_name.c_str(), key.c_str(), &tmp_err);
   GErrorWrapper::throwOnError(&tmp_err);
   return ret;
}


int Gfal2Context::get_opt_integer(const std::string & nmspace, const std::string & key)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    int ret = gfal2_get_opt_integer(cont->get(), nmspace.c_str(), key.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


std::string Gfal2Context::get_opt_string(const std::string & nmspace,
        const std::string & key)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    char* p = gfal2_get_opt_string(cont->get(), nmspace.c_str(), key.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return std::string(p);
}


boost::python::list Gfal2Context::get_opt_string_list(const std::string & nmspace, const std::string & key)
{
    GError * tmp_err = NULL;
    gsize size = 0;
    char** res;
    {
        ScopedGILRelease unlock;
        res = gfal2_get_opt_string_list(cont->get(), nmspace.c_str(), key.c_str(),
                &size, &tmp_err);
    }
    GErrorWrapper::throwOnError(&tmp_err);

    boost::python::list result;
    if (res) {
        for (size_t i = 0; i < size; i++)
            result.append(std::string(res[i]));
        g_strfreev(res);
    }
    return result;
}


bool Gfal2Context::get_opt_boolean(const std::string & nmspace, const std::string & key)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    const bool ret = gfal2_get_opt_boolean(cont->get(), nmspace.c_str(), key.c_str(),
            &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


int Gfal2Context::set_opt_integer(const std::string & nmspace, const std::string & key, int value)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    int ret = gfal2_set_opt_integer(cont->get(), nmspace.c_str(), key.c_str(), value, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


int Gfal2Context::set_opt_string(const std::string & nmspace, const std::string & key,
        const std::string & value)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    int ret = gfal2_set_opt_string(cont->get(), nmspace.c_str(), key.c_str(), (char*) value.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


int Gfal2Context::set_opt_string_list(const std::string & nmspace,
        const std::string & key, const boost::python::list &py_value)
{
    std::vector<std::string> value = convert_python_list_to_typed_list<std::string>(
            py_value);

    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    const int size_list = value.size();
    char* tab_ptr[size_list + 1];
    for (int i = 0; i < size_list; i++) {
        tab_ptr[i] = (char*) value[i].c_str();
    }
    tab_ptr[size_list] = NULL;

    int ret = gfal2_set_opt_string_list(cont->get(), nmspace.c_str(), key.c_str(),
            tab_ptr, size_list, &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


int Gfal2Context::set_opt_boolean(const std::string & nmspace, const std::string & key, bool val)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    int ret = gfal2_set_opt_boolean(cont->get(), nmspace.c_str(), key.c_str(), val,
            &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


int Gfal2Context::load_opts_from_file(const std::string & path)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    int ret = gfal2_load_opts_from_file(cont->get(), path.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


boost::python::list Gfal2Context::get_plugin_names(void)
{
    gchar** plugins;
    {
        ScopedGILRelease unlock;
        plugins = gfal2_get_plugin_names(cont->get());
    }

    int nplugins = g_strv_length(plugins);
    boost::python::list pyplugins;

    for (int i = 0; i < nplugins; ++i) {
        pyplugins.append(std::string(plugins[i]));
    }
    g_strfreev(plugins);

    return pyplugins;
}


int Gfal2Context::set_user_agent(const std::string & agent, const std::string & version)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    int ret = gfal2_set_user_agent(cont->get(), agent.c_str(), version.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


boost::python::tuple Gfal2Context::get_user_agent(void)
{
    const char* agent, *version;
    {
        ScopedGILRelease unlock;
        gfal2_get_user_agent(cont->get(), &agent, &version);
    }
    return boost::python::make_tuple(agent, version);
}


int Gfal2Context::add_client_info(const std::string& key, const std::string& value)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    int ret = gfal2_add_client_info(cont->get(), key.c_str(), value.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


int Gfal2Context::remove_client_info(const std::string& key)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    int ret = gfal2_remove_client_info(cont->get(), key.c_str(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


int Gfal2Context::clear_client_info(void)
{
    ScopedGILRelease unlock;
    GError * tmp_err = NULL;
    int ret = gfal2_clear_client_info(cont->get(), &tmp_err);
    GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


boost::python::dict Gfal2Context::get_client_info(void)
{
    GError* tmp_err = NULL;
    size_t nitems;
    {
        ScopedGILRelease unlock;
        nitems = gfal2_get_client_info_count(cont->get(), &tmp_err);
    }

    GErrorWrapper::throwOnError(&tmp_err);
    boost::python::dict dictionary;

    for (size_t i = 0; i < nitems; ++i) {
        const char *key, *value;
        gfal2_get_client_info_pair(cont->get(), i, &key, &value, &tmp_err);
        GErrorWrapper::throwOnError(&tmp_err);
        dictionary[key] = value;
    }

    return dictionary;
}


int Gfal2Context::cred_set(const std::string& url_prefix, const Cred& c)
{
    ScopedGILRelease unlock;
    GError* error = NULL;
    int res = gfal2_cred_set(cont->get(), url_prefix.c_str(), c.cred, &error);
    GErrorWrapper::throwOnError(&error);
    return res;
}


boost::python::tuple Gfal2Context::cred_get(const std::string& type, const std::string& url)
{
    GError* error = NULL;
    const char* prefix_match = NULL;
    const char* value;
    {
        ScopedGILRelease unlock;
        const char* value = gfal2_cred_get(cont->get(), type.c_str(), url.c_str(),
                                           &prefix_match, &error);
    }
    GErrorWrapper::throwOnError(&error);
    std::string svalue = (value) ? value : "";
    std::string sprefix_match = (prefix_match) ? prefix_match : "";
    return boost::python::make_tuple(svalue, sprefix_match);
}


int Gfal2Context::cred_clean()
{
    ScopedGILRelease unlock;
    GError* error = NULL;
    int res = gfal2_cred_clean(cont->get(), &error);
    GErrorWrapper::throwOnError(&error);
    return res;
}


void Gfal2Context::free()
{
    cont->free();
}

int Gfal2Context::cancel()
{
    ScopedGILRelease unlock;
    return gfal2_cancel(cont->get());
}


int PyGfal2::gfal_set_verbose_enum(GLogLevelFlags lvls)
{
    ScopedGILRelease unlock;
    gfal2_log_set_level(lvls);
    return 0;
}
