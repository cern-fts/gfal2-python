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
 

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include <gfal_api.h>
#include <transfer/gfal_transfer.h>

#include "gfal_boost_include.hpp"
#include "gfalt_params.h"


#ifndef GFALFILE_H
#define GFALFILE_H

enum gfal_verbose_levels{
    gfal_verbose_normal =GFAL_VERBOSE_NORMAL,
    gfal_verbose_verbose = GFAL_VERBOSE_VERBOSE,
    gfal_verbose_debug = GFAL_VERBOSE_VERBOSE | GFAL_VERBOSE_DEBUG,
    gfal_verbose_trace = GFAL_VERBOSE_TRACE | GFAL_VERBOSE_VERBOSE | GFAL_VERBOSE_DEBUG
};

class Gfal{
private:

    class GfalContextWrapper {
    public:
        gfal_context_t context;

        GfalContextWrapper() {
            GError* tmp_err=NULL;
            context = gfal2_context_new(&tmp_err);
            if(context == NULL)
                check_GError(&tmp_err);
        }
        ~GfalContextWrapper() {
            gfal2_context_free(context);
        }
    };

    boost::shared_ptr<GfalContextWrapper> cont;

public:
    Gfal(): cont(new GfalContextWrapper()) {}

    virtual ~Gfal(){}

    class Gstat : public stat {
    public:
        dev_t get_st_dev();

        ino_t get_st_ino();

        mode_t get_st_mode();

        nlink_t get_st_nlink();

        uid_t get_st_uid();

        gid_t get_st_gid();

        off_t get_st_size();

        time_t get_st_atime();

        time_t get_st_mtime();

        time_t get_st_ctime();

        std::string string_rep();

    } ;

    class GfalFile
    {
        public:
            GfalFile(const Gfal & context,
                     const std::string & path,
                     const std::string & flag);
            virtual ~GfalFile();
            /// wrapper to the gfal_read call
            std::string read(size_t count);
            /// position independant read call
            std::string pread(off_t offset, size_t count);
            /// wrapper to the gfal_write call
            ssize_t write(const std::string & str);
            /// position independant write call
            ssize_t pwrite(const std::string & str, off_t offset);

            /**
             * wrapper to the gfal_lseek call
             */
             off_t lseek(off_t offset, int flag=0);

            // Static global function

            /**
             * Wrap to the gfal_lstat call
             * */
        private:
            /* add your private declarations */
            boost::shared_ptr<GfalContextWrapper> cont;
            std::string path;
            std::string flag;

            int fd;
    };

    boost::shared_ptr<GfalContextWrapper> getContext() const {
        return cont ;
    }


    int cancel(){
        return gfal2_cancel(cont->context);
    }

    boost::shared_ptr<GfalFile> open(const std::string & path, const std::string &flag);
    boost::shared_ptr<GfalFile> file(const std::string & path, const std::string &flag);

    Gstat lstat(const std::string & path);

    Gstat stat_c(const std::string & path);

    int access(const std::string &, int flag);

    int chmod(const std::string &, mode_t mode);

    int unlink(const std::string &);

    int mkdir(const std::string &, mode_t mode);

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


    // transfer
    int filecopy(const std::string & src, const std::string & dst);
    // transfer
    int filecopy(const Gfalt_params & p, const std::string & src, const std::string & dst);

 //   int filecopy(const Gfalt_params &params, const std::string & src, const std::string & dst);
};

int gfal_set_verbose_enum(enum gfal_verbose_levels lvls);


boost::shared_ptr<Gfal> create_instance();


#endif /* GFALFILE_H */ 
