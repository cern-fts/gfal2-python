#!/usr/bin/python

import sys
import gfal2
import time
import errno
import sys
import unittest
from python_test_lib import *
import logging

log_level_value = logging.ERROR - (3 * 10)

gfal2.set_verbose(gfal2.verbose_level.debug)
root_logger = logging.getLogger()
root_logger.setLevel(log_level_value)
handler = logging.StreamHandler(sys.stderr)
handler.setLevel(log_level_value)

handler.setFormatter(logging.Formatter('%(levelname)s %(message)s'))
if sys.stderr.isatty():
            logging.addLevelName(logging.DEBUG, "\033[1;2m%-8s\033[1;m" % logging.getLevelName(logging.DEBUG))
            logging.addLevelName(logging.INFO, "\033[1;34m%-8s\033[1;m" % logging.getLevelName(logging.INFO))
            logging.addLevelName(logging.ERROR, "\033[1;31m%-8s\033[1;m" % logging.getLevelName(logging.ERROR))
            logging.addLevelName(logging.WARNING, "\033[1;33m%-8s\033[1;m" % logging.getLevelName(logging.WARNING))

root_logger.addHandler(handler)

context = gfal2.creat_context()
context.unlink(sys.argv[1])
