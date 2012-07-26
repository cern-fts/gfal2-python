#!/usr/bin/python

import sys
import gfal2
import time

import unittest
from python_test_lib import *

class Testgfal2_listdir(unittest.TestCase):

        def setUp(self):
            self.context = gfal2.creat_context()

	def get_valid_parent_dir(self, base_url):
		v= get_val(base_url);
		a = ''.join([v, "/testlistdir_random_", str(time.time()),'/'])
		return a

	def get_list_child_dir(self, parent_dir, n):
		res = []
		for i in range(n):
			tmp =  ''.join([parent_dir, "file_test_", str(i)])
                        self.context.mkdir(tmp,0755)
			res.append(tmp)
		return res;
		
	def test_listdir_lfn(self):
		p = self.get_valid_parent_dir("TEST_LFN_BASE")
                self.context.mkdir(p,0755)
                res = self.context.listdir(p)
		self.assertTrue( res == [], "test if dir is empty")
		l = self.get_list_child_dir(p, 10)
                size_dir = len(self.context.listdir(p))
		self.assertTrue( size_dir == 10, "must be a dir of %d elem but is %d"%(10, size_dir))
			
	def test_listdir_srm(self):
		p = self.get_valid_parent_dir("TEST_SRM_BASE")
                self.context.mkdir(p,0755)
                res = self.context.listdir(p)
		self.assertTrue( res == [], "test if dir is empty")
		l = self.get_list_child_dir(p, 10)
                size_dir = len(self.context.listdir(p))
		self.assertTrue( size_dir == 10, "must be a dir of %d elem but is %d"%(10, size_dir))

	
