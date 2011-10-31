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

/**
 * @file gfalcpp.cpp
 * @brief GFAL 2.0 main C++ bindings
 * @author Devresse Adrien
 * @date 26/10/2011
 * */
 
 #include <gfal_api.h>
 
 
 #include <string>
 #include "gerror_exception.h"
 
 void gfal_GError_to_exception(){
	 char buffer[2048];
	 
	 int code = gfal_posix_code_error();
	 if(code){
		 std::string err_msg(gfal_posix_strerror_r(buffer, 2048));
		 throw Gerror_exception(err_msg, code );
	 }
 }
 

