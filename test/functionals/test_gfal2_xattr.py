#!/usr/bin/python

import sys
import gfal2
import errno

import unittest
from python_test_lib import *

class Testgfal2_xattr(unittest.TestCase):

        def setUp(self):
            self.context = gfal2.creat_context()
	  
	def getxattr_valid(self, url, key):
                return self.context.getxattr(url, key)

	def getxattr_enoent(self, url, key):
		try:
                        self.context.getxattr(url, key)
			self.assertTrue(False, " is a valdi stat, must not be");
		except gfal2.GError, e:
			self.assertTrue(e.code() == errno.ENOENT, " must be a non existing file");	

	def test_replicas_srm(self):
		v= self.getxattr_valid(get_val("TEST_SRM_READ_VALID"), "user.replicas");
		self.assertTrue(v.find("rfio:") != -1, " must be a valid turl")
		self.getxattr_enoent(''.join([get_val("TEST_SRM_READ_VALID"), "_testenoent"]), "user.replicas");
		
