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

#include <iostream>
#include <sstream>
#include <errno.h>
#include "gerror_exception.h"


Gerror_exception::Gerror_exception(std::string msg, int error) 
	: message(msg) 
{
	this->_code = error;
}


Gerror_exception::Gerror_exception(const GError* gerr)
	: message(gerr->message) 
{
	this->_code = gerr->code;
}


Gerror_exception::~Gerror_exception()  throw() 
{
	
}


const char* Gerror_exception::what() const throw(){
	std::cout << " test" << message.c_str() << std::endl;
	return message.c_str();
}

std::string Gerror_exception::get_message() const {
	std::stringstream ss;
	ss << "Error: "<< this->_code << " "<< strerror(this->_code) << 
	"\n" << message << std::endl;
	return ss.str();
}


int Gerror_exception::code() const{
	return this->_code;
}

