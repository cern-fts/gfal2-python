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

#ifndef GSTAT_H
#define GSTAT_H

#include <cstring>
#include <sstream>
#include <sys/stat.h>

namespace PyGfal2 {

class Stat {
public:
    struct stat _st;

    Stat(){
        memset(&_st,0, sizeof(struct stat));
    }

    Stat(const Stat & orig){
        memcpy(&_st, &(orig._st), sizeof(struct stat));
    }

    dev_t get_st_dev(){
        return _st.st_dev;
    }

    ino_t get_st_ino(){
        return _st.st_ino;
    }

    mode_t get_st_mode(){
    return _st.st_mode;
    }

    nlink_t get_st_nlink() {
        return _st.st_nlink;
    }

    uid_t get_st_uid() {
        return _st.st_uid;
    }

    gid_t get_st_gid() {
        return _st.st_gid;
    }

    size_t get_st_size() {
        return _st.st_size;
    }

    time_t get_st_atime() {
        return _st.st_atime;
    }

    time_t get_st_mtime() {
        return _st.st_mtime;
    }

    time_t get_st_ctime() {
        return _st.st_ctime;
    }

    std::string __str__() {
        std::ostringstream res;
        res << "uid: " << get_st_uid() << '\n';
        res << "gid: " << get_st_gid() << '\n';
        res << "mode: " << std::oct << get_st_mode() << std::dec << '\n';
        res << "size: " << get_st_size() << '\n';
        res << "nlink: " << get_st_nlink() << '\n';
        res << "ino: " << get_st_ino() << '\n';
        res << "ctime: " << get_st_ctime() << '\n';
        res << "atime: " << get_st_atime() << '\n';
        res << "mtime: " << get_st_mtime() << '\n';

        return res.str();
    }

};

}

#endif /* GSTAT_H */
