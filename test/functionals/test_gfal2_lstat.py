#!/usr/bin/python

import sys
import gfal2

import unittest
import errno
from python_test_lib import *

class Testgfal2_lstat(unittest.TestCase):

        def setUp(self):
            self.context = gfal2.creat_context()
	  
	def lstat_valid(self, url):
		v= get_val(url);
                a= self.context.lstat(v);
		self.assertTrue(a.st_uid != 0 and a.st_gid != 0);

	def lstat_enoent(self, url):
		v= get_val(url);
		noent= ''.join([v, "_enoent"]) # create non enxisting file
		try:
                        a= self.context.lstat(noent);
			self.assertTrue(False, " is a valdi stat, must not be");
                except gfal2.GError, e:
			self.assertTrue(e.code() == errno.ENOENT, " must be a non existing file");			

	def test_lstat_srm(self):
		self.lstat_valid("TEST_SRM_LSTAT_VALID");
		self.lstat_enoent("TEST_SRM_LSTAT_VALID");

	
