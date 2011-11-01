#!/usr/bin/python

import sys
import gfal2
import time

import unittest
from python_test_lib import *

class Testgfal2_rename_unlink(unittest.TestCase):
	  
	def get_filename_valid(self, base_url):
		v= get_val(base_url);
		return ''.join([v,"/testfile_rename_", str(time.time())] );		

	def test_rename_simple_lfn(self):
		filename = self.get_filename_valid("TEST_LFN_BASE")
		filename2 = ''.join([filename,"_2"])		
		# impossible to write on lfc -> mkdir
		gfal2.mkdir(filename, 0755)
		gfal2.lstat(filename); # test if exist
		gfal2.rename(filename, filename2)
		gfal2.rmdir(filename2)	
			
	def test_rename_simple_srm(self):
		filename = self.get_filename_valid("TEST_SRM_BASE")
		filename2 = ''.join([filename,"_2"])
		f = gfal2.file(filename, 'w');
		f.write("simple test file for rename")
		del f # write a simple file
		gfal2.lstat(filename); # test if exist
		#gfal2.rename(filename, filename2)
		#gfal2.lstat(filename2)
		gfal2.unlink(filename)
	
