/*
* Copyright @ CERN, 2014-2015
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


#ifndef PYGIL_H
#define PYGIL_H

#include <boost/python.hpp>
#include <vector>

namespace PyGfal2 {

/**
 * GIL unlocker, used on stack
 */
class ScopedGILRelease {
public:
    inline ScopedGILRelease(){
        m_thread_state = PyEval_SaveThread();
    }

    inline ~ScopedGILRelease(){
        PyEval_RestoreThread(m_thread_state);
        m_thread_state = NULL;
    }

private:
    PyThreadState * m_thread_state;
};


/**
 * GIL locker, used on stack
 */
class ScopedGILLocker {

public:
    inline ScopedGILLocker(){
        gil_state = PyGILState_Ensure();
    }

    inline ~ScopedGILLocker(){
        PyGILState_Release(gil_state);
    }

private:
    PyGILState_STATE gil_state;
};

}

#endif // PYGIL_H
