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


#include <cstring>
#include <sstream>

#include "gfalfile.h"


Gfal::GDirent::GDirent() :
    _dir(),
    _end_of_directory(true) {
    memset(&_dir, 0, sizeof(struct dirent));
}


Gfal::GDirent::GDirent(struct dirent* entry) {
    _end_of_directory = (entry == NULL);
    if (!_end_of_directory) {
        memcpy(&_dir, entry, sizeof(struct dirent));
    }else{
        memset(&_dir, 0, sizeof(struct dirent));
    }
}

Gfal::GDirent::GDirent(const GDirent & orig){
    memcpy(&_dir, &orig._dir, sizeof(struct dirent));
}

ino_t Gfal::GDirent::get_d_ino() {
    return _dir.d_ino;
}

off_t Gfal::GDirent::get_d_off() {

        return _dir.d_off;
}


unsigned short Gfal::GDirent::get_d_reclen() {

        return _dir.d_reclen;
}

unsigned char Gfal::GDirent::get_d_type() {

        return _dir.d_type;
}


std::string Gfal::GDirent::get_d_name() {

        return std::string(_dir.d_name);
}

std::string Gfal::GDirent::string_rep() {

	std::ostringstream res;
    res << "inode: " << _dir.d_ino << std::endl;
    res << "offset: " << _dir.d_off << std::endl;
    res << "length: " << _dir.d_reclen << std::endl;
    res << "type: " << _dir.d_type << std::endl;
    res << "name: " << _dir.d_name << std::endl;

	return res.str();
}

