//      gfal_stat.cpp
//      
//      Copyright 2011 didi <didi@didi-laptop>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

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


