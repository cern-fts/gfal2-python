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
#include <sstream>
#include <cstring>
#include <cerrno>
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

