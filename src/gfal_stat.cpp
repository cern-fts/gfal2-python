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

#include "gfal_stat.h"


	
dev_t Gfal::Gstat::get_st_dev(){
	struct stat * st = this;	
	return st->st_dev;
}

ino_t Gfal::Gstat::get_st_ino(){
	struct stat * st = this;	
	return st->st_ino;
}

mode_t Gfal::Gstat::get_st_mode(){
	struct stat * st = this;	
	return st->st_mode;
}

nlink_t Gfal::Gstat::get_st_nlink(){
	struct stat * st = this;
	return st->st_nlink;
}

uid_t Gfal::Gstat::get_st_uid(){
	struct stat * st = this;	
	return st->st_uid;
}

gid_t Gfal::Gstat::get_st_gid(){
	struct stat * st = this;	
	return st->st_gid;
}

off_t Gfal::Gstat::get_st_size(){
	struct stat * st = this;	
	return st->st_size;
}

time_t Gfal::Gstat::get_st_atime(){
	struct stat * st = this;	
	return st->st_atime;
}

time_t Gfal::Gstat::get_st_mtime(){
	struct stat * st = this;	
	return st->st_mtime;
}

time_t Gfal::Gstat::get_st_ctime(){
	struct stat * st = this;	
	return st->st_ctime;
}

std::string Gfal::Gstat::string_rep() {
	struct stat * st = this;
	std::ostringstream res;
	res << "uid: " << st->st_uid << std::endl;
	res << "gid: " << st->st_gid << std::endl;
	res << "mode: " << std::oct << st->st_mode << std::endl;
	res << "size: " << st->st_size << std::endl;
	res << "nlink: " << st->st_nlink << std::endl;
	res << "ino: " << st->st_ino << std::endl;
	res << "ctime: " << st->st_ctime << std::endl;
	res << "atime: " << st->st_atime << std::endl;
	res << "mtime: " << st->st_mtime << std::endl;

	return res.str();
}


