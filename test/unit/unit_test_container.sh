#!/bin/bash

## setup environment variables for tests
# 

# consider that "make test" is executed in a build directory

echo "build path $1"

MODULE_DIR=$(readlink -f `pwd`/../../src/)

export PYTHONPATH=$MODULE_DIR:${PYTHONPATH}


echo "export python path $PYTHONPATH"

$2


