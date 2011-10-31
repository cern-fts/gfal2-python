//      gfal_stat.h
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


#ifndef GFAL_STAT_H
#define GFAL_STAT_H

#include <string>
#include <sys/stat.h>
#include <unistd.h>

#include "gfalfile.h"

namespace Gfal{

typedef struct _Gstat : stat {	
	
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

} Gstat;
	
}
#endif /* GFAL_STAT_H */ 
