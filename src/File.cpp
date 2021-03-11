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

#include "File.h"
#include "Gfal2Context.h"
#include "pyGIL.h"

#include <boost/python.hpp>
#include <fcntl.h>
#include <stdexcept>

using namespace PyGfal2;


static int convert_open_flag_py_to_cpp(const std::string & str)
{
    if (str.compare("rw") == 0 || str.compare("rw") == 0)
        return O_RDWR | O_CREAT;
    if (str.compare("r") == 0)
        return O_RDONLY;
    if (str.compare("w") == 0)
        return O_WRONLY | O_CREAT | O_TRUNC;
    throw std::runtime_error("Invalid open flag, must be r, w, or rw");
}


File::File(const Gfal2Context & context, const std::string & path,
        const std::string & flag) :
        cont(context.getContext()), path(path), flag(flag)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    int flags_i = convert_open_flag_py_to_cpp(flag);
    fd = gfal2_open(cont->get(), path.c_str(), flags_i, &tmp_err);
    if (fd <= 0)
        GErrorWrapper::throwOnError(&tmp_err);
}


File::~File()
{
    ScopedGILRelease unlock;
    (void) gfal2_close(cont->get(), fd, NULL);
}


std::string File::read(size_t count)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    std::vector<char> buf(count + 1); // vector on the heap for massive buffer size
    ssize_t ret = gfal2_read(cont->get(), fd, &(buf.front()), count, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);

    buf[ret] = '\0';
    return std::string(&(buf.front()), ret);
}


std::string File::pread(off_t offset, size_t count)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    std::vector<char> buf(count + 1); // vector on the heap for massive buffer size
    ssize_t ret = gfal2_pread(cont->get(), fd, &(buf.front()), count, offset, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);

    buf[ret] = '\0';
    return std::string(&(buf.front()), ret);
}


PyObject* File::read_bytes(size_t count)
{
    std::string result = read(count);
    return PyBytes_FromStringAndSize(result.c_str(), result.size());
}


PyObject* File::pread_bytes(off_t offset, size_t count)
{
    std::string result = pread(offset, count);
    return PyBytes_FromStringAndSize(result.c_str(), result.size());
}


ssize_t File::write(const std::string & str)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    const size_t s_str = str.size();

    ssize_t ret = gfal2_write(cont->get(), fd, str.c_str(), s_str, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


ssize_t File::pwrite(const std::string & str, off_t offset)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    const size_t s_str = str.size();

    ssize_t ret = gfal2_pwrite(cont->get(), fd, str.c_str(), s_str, offset, &tmp_err);
    if (ret < 0)
        GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}


off_t File::lseek(off_t offset, int flag)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    off_t ret = gfal2_lseek(cont->get(), fd, offset, flag, &tmp_err);
    if (ret == ((off_t) 0) - 1)
        GErrorWrapper::throwOnError(&tmp_err);
    return ret;
}

