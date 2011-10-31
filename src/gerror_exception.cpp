//      gerror_exception.cpp
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


#include "gerror_exception.h"


Gerror_exception::Gerror_exception(std::string msg, int error) throw()
	: message(msg)
{
	
}


Gerror_exception::Gerror_exception(const GError* gerr) throw()
	: message(gerr->message)
{
	this->code = gerr->code;
}


Gerror_exception::~Gerror_exception() throw()
{
	
}


const char* Gerror_exception::what() const throw(){
	return message.c_str();
}
