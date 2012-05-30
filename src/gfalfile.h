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
 

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include "gfal_boost_include.hpp"


#include "gfal_stat.h"
#include "gerror_exception.h"

#ifndef GFALFILE_H
#define GFALFILE_H

class Gfal{

public:

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
		GfalFile(const std::string & path, const std::string &flag);
		virtual ~GfalFile();
		/**
		 * wrapper to the gfal_read call
		 */
		std::string read(size_t count);
		/**
		 * wrapper to the gfal_write call
		 */
		ssize_t write(const std::string & str) ;
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
		std::string path;
		std::string flag;
		
		int fd;
};


boost::shared_ptr<GfalFile> open(const std::string & path, const std::string &flag);


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


// extended attributes

std::string getxattr(const std::string & file, const std::string & key);

int setxattr(const std::string & file, const std::string & key, const std::string & value, int flag);

boost::python::list listxattr(const std::string & file );

/*
int set_parameter_string(const std::string & namespc, const std::string & key,  const std::string & str);

int set_parameter_bool(const std::string & namespc, const std::string & key, const bool  b);

bool get_parameter_bool(const std::string & namespc, const std::string & key);

std::string get_parameter_string(const std::string & namespc, const std::string & key);
*/
};



#endif /* GFALFILE_H */ 
