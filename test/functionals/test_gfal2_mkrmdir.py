#!/usr/bin/python

import sys
import gfal2
import time
import errno

import unittest
from python_test_lib import *

class Testgfal2_mkrmdir(unittest.TestCase):

        def setUp(self):
            self.context = gfal2.creat_context()

	def mkrmdir_valid(self, url):
		try:
                        self.context.rmdir(url); # must never be read
			self.assertTrue(False)
		except:
			pass
		
                self.context.mkdir(url,0775)
                self.context.rmdir(url)
		try:
                        self.context.rmdir(url)
			self.assertTrue(False, "  reach a non possible success for rmdir")	
		except gfal2.GError, e:
			self.assertTrue(e.code() == errno.ENOENT, " error is not a non existing dir")		

	def get_valid_file(self, base_url):
		v= get_val(base_url);
		cmpl= str(time.time())
		return ''.join([v, "/testmkdir_random", cmpl])
		
	def test_mkrmdir_srm(self):
		f = self.get_valid_file("TEST_SRM_BASE");
		self.mkrmdir_valid(f)

	
