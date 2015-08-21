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

#ifndef GFAL2CONTEXT_h
#define GFAL2CONTEXT_h

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <dirent.h>

#include <unistd.h>
#include <vector>

#include <gfal_api.h>
#include <transfer/gfal_transfer.h>

#include "Directory.h"
#include "Dirent.h"
#include "File.h"
#include "GfaltParams.h"
#include "Stat.h"
#include "pyGIL.h"

namespace PyGfal2 {

class GfalContextWrapper
{
public:
    gfal2_context_t context;

    GfalContextWrapper()
    {
        GError* tmp_err = NULL;
        context = gfal2_context_new(&tmp_err);
        if (context == NULL)
            GErrorWrapper::throwOnError(&tmp_err);
    }
    ~GfalContextWrapper()
    {
        gfal2_context_free(context);
    }
};


class Gfal2Context
{
private:
    boost::shared_ptr<GfalContextWrapper> cont;

public:
    Gfal2Context() {
        ScopedGILRelease unlock;
        cont = boost::shared_ptr<GfalContextWrapper>(new GfalContextWrapper);
    }

    virtual ~Gfal2Context() {
    }

    static Gfal2Context creat_context(void) {
        return PyGfal2::Gfal2Context();
    }

    boost::shared_ptr<GfalContextWrapper> getContext() const {
        return cont ;
    }


    int cancel();

    boost::shared_ptr<File> open(const std::string & path, const std::string &flag);
    boost::shared_ptr<File> file(const std::string & path, const std::string &flag);

    boost::shared_ptr<Directory> opendir(const std::string & path);
    boost::shared_ptr<Directory> directory(const std::string & path);

    Stat lstat(const std::string & path);

    Stat stat_c(const std::string & path);

    int access(const std::string &, int flag);

    int chmod(const std::string &, mode_t mode);

    int unlink(const std::string &);

    boost::python::list unlink_list(const boost::python::list& files);

    int mkdir(const std::string &, mode_t mode);

    int mkdir_rec(const std::string &, mode_t mode);

    int rmdir(const std::string &);


    boost::python::list listdir(const std::string &);

    int rename(const std::string & src, const std::string & dest);

    std::string readlink(const std::string & path);

    int symlink(const std::string & oldpath, const std::string & newpath);

    std::string checksum(const std::string & uri, const std::string & chk_type,
                     off_t start_offset, size_t data_length);

    std::string checksum(const std::string & uri, const std::string & chk_type);

    // extended attributes

    std::string getxattr(const std::string & file, const std::string & key);

    int setxattr(const std::string & file, const std::string & key, const std::string & value, int flag);

    boost::python::list listxattr(const std::string & file );


    // parameters
    int get_opt_integer(const std::string & nmspace, const std::string & key);

    std::string get_opt_string(const std::string & nmspace, const std::string & key);

    boost::python::list get_opt_string_list(const std::string & nmspace, const std::string & key);

    bool get_opt_boolean(const std::string & nmspace, const std::string & key);

    int set_opt_integer(const std::string & nmspace, const std::string & key, int value);

    int set_opt_string(const std::string & nmspace, const std::string & key, const std::string & value);

    int set_opt_string_list(const std::string & nmspace, const std::string & key, const boost::python::list  & value);

    int set_opt_boolean(const std::string & nmspace, const std::string & key, bool val);

    int load_opts_from_file(const std::string & path);

    int set_user_agent(const std::string & agent, const std::string & version);

    boost::python::list get_plugin_names(void);

    // Client-side additional information
    boost::python::tuple get_user_agent(void);

    int add_client_info(const std::string& key, const std::string& value);
    int remove_client_info(const std::string& key);
    int clear_client_info(void);
    boost::python::dict get_client_info(void);

    // transfer
    int filecopy(const std::string & src, const std::string & dst);
    int filecopy(const GfaltParams & p, const std::string & src, const std::string & dst);

    boost::python::object filecopy(const boost::python::list& srcs,
            const boost::python::list& dsts);
    boost::python::object filecopy(const GfaltParams & p, const boost::python::list& srcs,
            const boost::python::list& dsts);
    boost::python::object filecopy(const GfaltParams & p, const boost::python::list& srcs,
            const boost::python::list& dsts,
            const boost::python::list& checksums);

    // bring online and related
    boost::python::tuple bring_online(const std::string& path, time_t pintime,
            time_t timeout, bool async);

    int bring_online_poll(const std::string& path, const std::string& token);

    int release(const std::string& path, const std::string& token);

    boost::python::tuple bring_online_list(const boost::python::list& files, time_t pintime,
            time_t timeout, bool async);

    boost::python::list bring_online_poll_list(const boost::python::list& files, const std::string& token);

    boost::python::list release_list(const boost::python::list& files, const std::string& token);

    int abort_bring_online(const std::string& path, const std::string& token);

    boost::python::list abort_bring_online_list(const boost::python::list& files, const std::string& token);
};

int gfal_set_verbose_enum(GLogLevelFlags lvls);

} // PyGfal2 namespace

#endif /* GFAL2CONTEXT_h */
