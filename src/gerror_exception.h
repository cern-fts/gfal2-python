//      gerror_exception.h
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


#ifndef GERROR_EXCEPTION_H
#define GERROR_EXCEPTION_H

#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <glib.h>

class Gerror_exception : public std::exception
{
	public:
		Gerror_exception(std::string path, int error);
		Gerror_exception(const GError* gerr) ;
		
		virtual ~Gerror_exception() throw();
	
		const char * what() const throw();
		
		int code() const;
	private:
		int _code;
		std::string message;
		/* add your private declarations */
};

#endif /* GERROR_EXCEPTION_H */ 
