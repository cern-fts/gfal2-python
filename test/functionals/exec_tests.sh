#!/bin/bash

mydir=$(dirname $0)
source $mydir/setup_env_test.sh

$mydir/test-gfal2-all.py


