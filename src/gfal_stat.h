/*
 * Copyright (c) Members of the EGEE Collaboration. 2004.
 * See http://www.eu-egee.org/partners/ for details on the copyright holders.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 


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
