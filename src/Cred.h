/*
 * Copyright (c) CERN 2013-2021
 *
 * Copyright (c) Members of the EMI Collaboration. 2010-2013
 *  See  http://www.eu-emi.eu/partners for details on the copyright
 *  holders.
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

#ifndef GCRED_H
#define GCRED_H

#include <common/gfal_cred_mapping.h>
#include "GErrorWrapper.h"

namespace PyGfal2 {

// Wrapper for gfal2_cred_t
class Cred
{
private:
    gfal2_cred_t* cred;

public:
    Cred(const std::string& type, const std::string& value) {
        cred = gfal2_cred_new(type.c_str(), value.c_str());
    }

    virtual ~Cred() {
        gfal2_cred_free(cred);
    }

    std::string get_type() {
        return std::string(cred->type);
    }

    std::string get_value() {
        return std::string(cred->value);
    }

    friend class Gfal2Context;
};

}

#endif /* GCRED_H */
