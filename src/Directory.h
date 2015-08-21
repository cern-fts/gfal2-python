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

#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include "Dirent.h"
#include <boost/python.hpp>

namespace PyGfal2 {

class Gfal2Context;
class GfalContextWrapper;

class Directory : protected boost::noncopyable
{
    public:
        Directory(const Gfal2Context & context,
                    const std::string & path);
        virtual ~Directory();

        boost::python::tuple readpp();
        Dirent read();

    private:
        boost::shared_ptr<GfalContextWrapper> cont;
        std::string path;
        DIR* d;
};

}

#endif /* DIRECTORY_H_ */
