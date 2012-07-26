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

#ifndef GFALT_PARAMS_H
#define GFALT_PARAMS_H

#include <gfal_api.h>
#include <transfer/gfal_transfer.h>

#include "gfalcpp.h"

class Gfalt_params
{
private:
    gfalt_params_t params;
public:
    Gfalt_params(){
        GError * tmp_err=NULL;
        gfalt_params_handle_new(&tmp_err);
        check_GError(&tmp_err);
    }

    Gfalt_params(const Gfalt_params & s){
        GError * tmp_err=NULL;
        params = gfalt_params_handle_copy(s.params,&tmp_err );
        check_GError(&tmp_err);
    }

    virtual ~Gfalt_params(){
        gfalt_params_handle_delete(params,NULL);
    }

    void set_timeout(){
        guint64 timeout;
        GError * tmp_err=NULL;
        gfalt_set_timeout(params, timeout, &tmp_err);
        check_GError(&tmp_err);
    }

    guint64 get_timeout(){
        GError * tmp_err=NULL;
        guint64 res = gfalt_get_timeout(params, &tmp_err);
        check_GError(&tmp_err);
        return res;
    }

    void set_checksum_check(bool checksum_check){
        GError * tmp_err=NULL;
        gfalt_set_checksum_check(params, checksum_check, &tmp_err);
        check_GError(&tmp_err);
    }


    bool get_checksum_check(){
        GError * tmp_err=NULL;
        bool res = gfalt_get_checksum_check(params,  &tmp_err);
        check_GError(&tmp_err);
        return res;
    }

    friend class Gfal;
};

#endif // GFALT_PARAMS_H
