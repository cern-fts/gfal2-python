/*
 * Copyright (c) CERN 2013-2015
 -
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

namespace PyGfal2 {

extern PyObject *GErrorPyType;

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

		/**
		 * Convenience function.
		 * Throws a GErrorWrapper if err is set
		 * Otherwise, it just returns
		 */
		static void throwOnError(GError**err);

	private:
		std::string _message;
		int _code;
};

/**
 * Creates the _type_ GErrorException (inherits from Python's Exception)
 */
PyObject* createGErrorExceptionType(boost::python::scope&);

/**
 * Translate a list of GError to a list of GErrorException
 */
void GError2PyError(boost::python::list& pyerrors, size_t nerrors, GError** g_errors);

}

#endif /* GERRORWRAPPER_EXCEPTION_H */
