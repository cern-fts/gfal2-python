/*
* Copyright @ Members of the EMI Collaboration, 2013.
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
 


#ifndef GERRORWRAPPER_EXCEPTION_H
#define GERRORWRAPPER_EXCEPTION_H

#include <boost/python.hpp>
#include <exception>
#include <glib.h>
#include <stdexcept>
#include <typeinfo>

/**
 * Exception to be thrown by the wrapper code
 */
class GErrorWrapper : public std::exception
{
	public:
        GErrorWrapper(const std::string &msg, int error);
        GErrorWrapper(const GError* gerr);
		
		virtual ~GErrorWrapper() throw();
	
		const char * what() const throw();
		int code() const;

	private:
		std::string _message;
		int _code;
};

/**
 * Create GErrorException (inherits from Python's Exception)
 */
PyObject* createGErrorException(boost::python::scope&);

#endif /* GERRORWRAPPER_EXCEPTION_H */
