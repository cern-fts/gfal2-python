#!/bin/bash

mydir=$(dirname $0)
source $mydir/setup_env_test.sh


MODULE_DIR=$(readlink -f `pwd`/../../src/)

export PYTHONPATH=$MODULE_DIR:${PYTHONPATH}

$mydir/test-gfal2-all.py


