#!/bin/bash

##
#

export MY_VO="dteam"


export TEST_SRM_BASE="root://dpmhead-rc.cern.ch/dpm/cern.ch/home/${MY_VO}/gfal2-tests/"
#export TEST_SRM_BASE="srm://dpmhead-rc.cern.ch:8446/manager/srmv2.2?SFN=/dpm/cern.ch/home/${MY_VO}/gfal2-tests/"

export TEST_FILE_CONTENT="Hello world" 

# lstat
export TEST_SRM_LSTAT_VALID=$TEST_SRM_BASE/teststat0011

# open/read/write
export TEST_SRM_READ_VALID=$TEST_SRM_BASE/testread0011
