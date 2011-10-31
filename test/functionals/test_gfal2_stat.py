#!/usr/bin/python

import sys
import gfal2

import unittest
from python_test_lib import *

class Testgfal2_stat(unittest.TestCase):
	  
	def stat_valid(self, url):
		v= get_val(url);
		a= gfal2.stat(v);
		self.assertTrue(a.st_uid != 0 and a.st_gid != 0);

	def test_stat_lfn(self):
		self.stat_valid("TEST_LFN_LSTAT_VALID");
			
	def test_lstat_srm(self):
		self.stat_valid("TEST_SRM_LSTAT_VALID");

	
