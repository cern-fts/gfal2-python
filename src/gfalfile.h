//      gfalfile.h
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

#include <iostream>
#include <string>



#include "gfal_stat.h"
#include "gerror_exception.h"

#ifndef GFALFILE_H
#define GFALFILE_H

namespace Gfal{

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


struct _Gstat lstat(const std::string & path);

struct _Gstat stat_c(const std::string & path);

int access(const std::string &, int flag);

int chmod(const std::string &, mode_t mode);

int unlink(const std::string &);



}
#endif /* GFALFILE_H */ 
