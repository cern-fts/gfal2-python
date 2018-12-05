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

#ifndef GFILE_H_
#define GFILE_H_

#include "Python.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <sys/stat.h>

namespace PyGfal2 {

class Gfal2Context;
class GfalContextWrapper;

class File: protected boost::noncopyable
{
public:
    File(const Gfal2Context & context, const std::string & path,
            const std::string & flag);
    virtual ~File();
#if PY_MAJOR_VERSION >= 3 /* workaround for https://github.com/boostorg/python/issues/85 */
    const char * read(size_t count);
#else /* python 2.x */
    std::string read(size_t count);
#endif
    /// position independent read call
    std::string pread(off_t offset, size_t count);
    /// wrapper to the gfal_write call
    ssize_t write(const std::string & str);
    /// position independent write call
    ssize_t pwrite(const std::string & str, off_t offset);

    /// wrapper to the gfal_lseek call
    off_t lseek(off_t offset, int flag = 0);

private:
    boost::shared_ptr<GfalContextWrapper> cont;
    std::string path;
    std::string flag;

    int fd;
};

}

#endif /* GFILE_H_ */
