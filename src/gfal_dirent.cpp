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


#include <cstring>
#include <sstream>

#include "gfalfile.h"


ino_t Gfal::Gdirent::get_d_ino() {
	struct dirent * ent = this;
	return ent->d_ino;
}

off_t Gfal::Gdirent::get_d_off() {
        struct dirent * ent = this;
        return ent->d_off;
}


unsigned short Gfal::Gdirent::get_d_reclen() {
        struct dirent * ent = this;
        return ent->d_reclen;
}

unsigned char Gfal::Gdirent::get_d_type() {
        struct dirent * ent = this;
        return ent->d_type;
}


std::string Gfal::Gdirent::get_d_name() {
        struct dirent * ent = this;
        return ent->d_name;
}

