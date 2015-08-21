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

#ifndef GDIRENT_H
#define GDIRENT_H

#include <cstring>
#include <dirent.h>
#include <sstream>

namespace PyGfal2 {

class Dirent{
public:
    struct dirent _dir;
    bool _end_of_directory;

    bool isValid() { return !_end_of_directory; }

    Dirent(): _dir(), _end_of_directory(true) {
        memset(&_dir, 0, sizeof(struct dirent));
    }

    Dirent(struct dirent* entry) {
        _end_of_directory = (entry == NULL);
        if (!_end_of_directory) {
            memcpy(&_dir, entry, sizeof(struct dirent));
        }
        else {
            memset(&_dir, 0, sizeof(struct dirent));
        }
    }

    Dirent(const Dirent & orig): _end_of_directory(orig._end_of_directory) {
        memcpy(&_dir, &orig._dir, sizeof(struct dirent));
    }

    ino_t get_d_ino() {
        return _dir.d_ino;
    }

    off_t get_d_off() {
#ifdef __linux__
        return _dir.d_off;
#else
        return 0;
#endif
    }

    unsigned short get_d_reclen() {
        return _dir.d_reclen;
    }

    unsigned char get_d_type() {
        return _dir.d_type;
    }

    std::string get_d_name() {
        return std::string(_dir.d_name);
    }

    std::string __str__() {
        std::ostringstream res;
        res << "inode: " << _dir.d_ino << std::endl;
#ifdef __linux__
        res << "offset: " << _dir.d_off << std::endl;
#endif
        res << "length: " << _dir.d_reclen << std::endl;
        res << "type: " << _dir.d_type << std::endl;
        res << "name: " << _dir.d_name << std::endl;

        return res.str();
    }
};
}

#endif /* GDIRENT_H */
