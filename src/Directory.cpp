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

#include "Gfal2Context.h"
#include "Directory.h"
#include "pyGIL.h"

#include <gfal_api.h>

using namespace PyGfal2;


Directory::Directory(const Gfal2Context & context, const std::string & path) :
        cont(context.getContext()), path(path)
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;
    d = gfal2_opendir(cont->get(), path.c_str(), &tmp_err);
    if (d == NULL)
        GErrorWrapper::throwOnError(&tmp_err);
}


Directory::~Directory()
{
    ScopedGILRelease unlock;
    (void) gfal2_closedir(cont->get(), d, NULL);
}


boost::python::tuple Directory::readpp()
{
    GError* tmp_err = NULL;
    Dirent dirent;
    Stat stat;

    {
        ScopedGILRelease unlock;
        dirent = gfal2_readdirpp(cont->get(), d, &stat._st, &tmp_err);
    }

    if (dirent.isValid() == false) {
        GErrorWrapper::throwOnError(&tmp_err);
        return boost::python::make_tuple(boost::python::object(), boost::python::object());
    }

    return boost::python::make_tuple(dirent, stat);
}


Dirent Directory::read()
{
    ScopedGILRelease unlock;
    GError* tmp_err = NULL;

    Dirent dirent(gfal2_readdir(cont->get(), d, &tmp_err));
    GErrorWrapper::throwOnError(&tmp_err);
    return dirent;
}
