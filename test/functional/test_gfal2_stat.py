#!/usr/bin/python

import sys
import gfal2
import errno

import unittest
from python_test_lib import *

class Testgfal2_stat(unittest.TestCase):

        def setUp(self):
            self.context = gfal2.creat_context()

	def stat_valid(self, url):
		v= get_val(url);
                a= self.context.stat(v);
		self.assertTrue(a.st_uid != 0 and a.st_gid != 0);

	def stat_enoent(self, url):
		v= get_val(url);
		noent= ''.join([v, "_enoent"]) # create non enxisting file
		try:
                        a= self.context.stat(noent);
			self.assertTrue(False, " is a valdi stat, must not be");
		except gfal2.GError, e:
			self.assertTrue(e.code() == errno.ENOENT, " must be a non existing file");	

	def test_lstat_srm(self):
		self.stat_valid("TEST_SRM_LSTAT_VALID");
		self.stat_enoent("TEST_SRM_LSTAT_VALID");

	
