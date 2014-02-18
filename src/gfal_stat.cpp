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


Gfal::GStat::GStat(){
    memset(&_st,0, sizeof(struct stat));
}

Gfal::GStat::GStat(const GStat & orig){
    memcpy(&_st, &(orig._st), sizeof(struct stat));
}


	
dev_t Gfal::GStat::get_st_dev(){
    return _st.st_dev;
}

ino_t Gfal::GStat::get_st_ino(){

    return _st.st_ino;
}

mode_t Gfal::GStat::get_st_mode(){

    return _st.st_mode;
}

nlink_t Gfal::GStat::get_st_nlink(){
    return _st.st_nlink;
}

uid_t Gfal::GStat::get_st_uid(){

    return _st.st_uid;
}

gid_t Gfal::GStat::get_st_gid(){

    return _st.st_gid;
}

size_t Gfal::GStat::get_st_size(){

    return _st.st_size;
}

time_t Gfal::GStat::get_st_atime(){

    return _st.st_atime;
}

time_t Gfal::GStat::get_st_mtime(){

    return _st.st_mtime;
}

time_t Gfal::GStat::get_st_ctime(){

    return _st.st_ctime;
}

std::string Gfal::GStat::string_rep() {
	std::ostringstream res;
    res << "uid: " << _st.st_uid << '\n';
    res << "gid: " << _st.st_gid << '\n';
    res << "mode: " << std::oct << _st.st_mode << '\n';
    res << "size: " << _st.st_size << '\n';
    res << "nlink: " << _st.st_nlink << '\n';
    res << "ino: " << _st.st_ino << '\n';
    res << "ctime: " << _st.st_ctime << '\n';
    res << "atime: " << _st.st_atime << '\n';
    res << "mtime: " << _st.st_mtime << '\n';

	return res.str();
}


