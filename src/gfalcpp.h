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
 


#ifndef GFALCPP_H
#define GFALCPP_H

#include "gfal_boost_include.hpp"

void gfal_GError_to_exception();

void check_GError(GError ** err);
 
 
/**
 * GIL unlocker, used on stack
 */
class Gfal_scopedGILRelease{
	
public:
    inline Gfal_scopedGILRelease(){
        m_thread_state = PyEval_SaveThread();
    }

    inline ~Gfal_scopedGILRelease(){
        PyEval_RestoreThread(m_thread_state);
        m_thread_state = NULL;
    }

private:
    PyThreadState * m_thread_state;
    
};

#endif
