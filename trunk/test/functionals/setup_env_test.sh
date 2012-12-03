#!/bin/bash

##
#

export MY_VO="testers.eu-emi.eu"

export TEST_LFN_BASE="lfn:/grid/${MY_VO}/"

export TEST_SRM_BASE="srm://cvitbdpm1.cern.ch/dpm/cern.ch/home/${MY_VO}/gfal2-tests/"

export TEST_FILE_CONTENT="Hello world" 
export LCG_GFAL_INFOSYS=certtb-bdii-top.cern.ch:2170
export LFC_HOST=cvitblfc1.cern.ch

# lstat
export TEST_LFN_LSTAT_VALID=$TEST_LFN_BASE/teststat0011

export TEST_SRM_LSTAT_VALID=$TEST_SRM_BASE/teststat0011

# open/read/write
export TEST_LFN_READ_VALID=$TEST_LFN_BASE/testread0011


export TEST_SRM_READ_VALID=$TEST_SRM_BASE/testread0011
