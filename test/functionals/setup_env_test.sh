#!/bin/bash

##
#

export TEST_LFN_BASE="lfn:/grid/dteam/"

export TEST_SRM_BASE="srm://cvitbdpm1.cern.ch/dpm/cern.ch/home/dteam/gfal2-tests/"

export TEST_FILE_CONTENT="Hello world" 

# lstat
export TEST_LFN_LSTAT_VALID=$TEST_LFN_BASE/teststat0011

export TEST_SRM_LSTAT_VALID=$TEST_SRM_BASE/teststat0011

# open/read/write
export TEST_LFN_READ_VALID=$TEST_LFN_BASE/testread0011


export TEST_SRM_READ_VALID=$TEST_SRM_BASE/testread0011
