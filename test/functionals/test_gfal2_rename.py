#!/usr/bin/python

import sys
import gfal2
import time
import errno

import unittest
from python_test_lib import *

class Testgfal2_rename_unlink(unittest.TestCase):

        def setUp(self):
            self.context = gfal2.creat_context()

	def get_filename_valid(self, base_url):
		v= get_val(base_url);
		return ''.join([v,"/testfile_rename_", str(time.time())] );		

	def test_rename_simple_lfn(self):
		filename = self.get_filename_valid("TEST_LFN_BASE")
		filename2 = ''.join([filename,"_2"])		
		# impossible to write on lfc -> mkdir
                self.context.mkdir(filename, 0755)
                self.context.lstat(filename); # test if exist
                self.context.rename(filename, filename2)
		try:
                        self.context.rename(filename, filename2)	# verify that a double rename fail
			self.assertTrue(False, "must not be a valid rename")
		except gfal2.GError, e:
			self.assertTrue(e.code() == errno.ENOENT, "must not be a valid rename")	
                self.context.rmdir(filename2)
			
	def test_rename_simple_srm(self):
		filename = self.get_filename_valid("TEST_SRM_BASE")
		filename2 = ''.join([filename,"_2"])
                f = self.context.file(filename, 'w');
		f.write("simple test file for rename")
		del f # write a simple file
                self.context.lstat(filename); # test if exist
		#gfal2.rename(filename, filename2)
		#gfal2.lstat(filename2)
                self.context.unlink(filename)
	
