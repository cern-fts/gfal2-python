#!/usr/bin/python

import sys
import gfal2

import unittest
from python_test_lib import *

class Testgfal2_file(unittest.TestCase):
	  
	def file_valid(self, url, flags):
		v= get_val(url);
		a= gfal2.file(v,flags);
		return a;		

	def test_read_simple_lfn(self):
		a= self.file_valid("TEST_LFN_READ_VALID","r");
		rs = a.read(500).strip();
		var = str(get_val("TEST_FILE_CONTENT"));
		self.assertTrue(rs == var, "must be %s but is %s."%(var, rs));
			
	def test_read_simple_srm(self):
		a= self.file_valid("TEST_SRM_READ_VALID",'r');
		rs = a.read(500).strip();
		var = get_val("TEST_FILE_CONTENT");
		self.assertTrue(rs == var, "must be %s but is %s."%(var, rs));
	
