#!/usr/bin/python

import sys
import gfal2
import time
import os

import unittest
from python_test_lib import *

class Testgfal2_file(unittest.TestCase):

        def setUp(self):
            self.context = gfal2.creat_context()

	def file_valid(self, url, flags):
		v= get_val(url);
                a= self.context.file(v,flags);
		return a;		

	def open_valid(self, url, flags): # same than file but with open func
		v= get_val(url);
                a= self.context.open(v,flags);
		return a;
			
	def test_read_simple_srm(self):
		a= self.file_valid("TEST_SRM_READ_VALID",'r');
		rs = a.read(500).strip();
		var = get_val("TEST_FILE_CONTENT");
		self.assertTrue(rs == var, "must be %s but is %s."%(var, rs));
		
	def test_read_compare_local(self):
		v = "file:///tmp/myfileread_" + str(time.time())	
                f1 = self.context.file(v,"w")
		var = os.urandom(1000)
		f1.write(var)
		del f1
                self.context.chmod(v, 0755)
                f2 = self.context.file(v,"r")
		var2 = f2.read(2000);
		self.assertTrue(var == var2, " read must be the same than write first %s second %s"%(var,var2))
		del f2
                self.context.unlink(v)
		
	def test_read_compare_local_with_open(self):
		v = "file:///tmp/myfileread_with_open_" + str(time.time())	
                f1 = self.context.open(v,"w")
		var = os.urandom(1000)
		f1.write(var)
		del f1
                self.context.chmod(v, 0755)
                f2 = self.context.open(v,"r")
		var2 = f2.read(2000);
		self.assertTrue(var == var2, " read must be the same than write first %s second %s"%(var,var2))
		del f2
                self.context.unlink(v)
		
	def test_read_compare_srm(self):
		v = "".join([get_val("TEST_SRM_BASE"), "/test_read_content_" ,str(time.time())])	
                f1 = self.context.file(v,"w")
		var = os.urandom(1000)
		f1.write(var)
		del f1
                self.context.chmod(v, 0755)
                f2 = self.context.file(v,"r")
		var2 = f2.read(2000);
		self.assertTrue(var == var2, " read must be the same than write first %s second %s"%(var,var2))
		del f2
                self.context.unlink(v)
		
