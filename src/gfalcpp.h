/*
* Copyright @ Members of the EMI Collaboration, 2010.
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


#ifndef GFALCPP_H
#define GFALCPP_H

#include "gfal_boost_include.hpp"


void check_GError(GError ** err);

// extract typed array from python array, throw type error in case of bad convertion
template <typename T>
std::vector<T> convert_python_list_to_typed_list(const boost::python::list & l){
    std::vector<T> res;
    for(int i =0; i < boost::python::len(l); ++i){
        res.push_back(boost::python::extract<T>(l[i]));
    }
    return res;
}
 
namespace GfalPy{ 
 
/**
 * GIL unlocker, used on stack
 */
class scopedGILRelease{
	
public:
    inline scopedGILRelease(){
        m_thread_state = PyEval_SaveThread();
    }

    inline ~scopedGILRelease(){
        PyEval_RestoreThread(m_thread_state);
        m_thread_state = NULL;
    }

private:
    PyThreadState * m_thread_state;
    
};



/**
 * GIL locker, used on stack
 */
class scopedGILLocker{
	
public:
    inline scopedGILLocker(){
        gil_state = PyGILState_Ensure();
    }

    inline ~scopedGILLocker(){
        PyGILState_Release(gil_state);
    }

private:
    PyGILState_STATE gil_state;
    
};

}

typedef GfalPy::scopedGILRelease Gfal_scopedGILRelease;
typedef GfalPy::scopedGILLocker Gfal_scopedGILLocker;


#endif
