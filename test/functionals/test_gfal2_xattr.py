#!/usr/bin/python

import sys
import gfal2
import errno

import unittest
from python_test_lib import *

class Testgfal2_xattr(unittest.TestCase):
	  
	def getxattr_valid(self, url, key):
		return gfal2.getxattr(url, key)

	def getxattr_enoent(self, url, key):
		try:
			gfal2.getxattr(url, key)
			self.assertTrue(False, " is a valdi stat, must not be");
		except gfal2.GError, e:
			self.assertTrue(e.code() == errno.ENOENT, " must be a non existing file");	

	def test_replicas_lfn(self):
		v= self.getxattr_valid(get_val("TEST_LFN_READ_VALID"), "user.replicas");
		self.assertTrue(v.find("srm:") != -1, " must be a valid surl")
		self.getxattr_enoent(''.join([get_val("TEST_LFN_READ_VALID"), "_testenoent"]), "user.replicas");
			
	def test_replicas_srm(self):
		v= self.getxattr_valid(get_val("TEST_SRM_READ_VALID"), "user.replicas");
		self.assertTrue(v.find("rfio:") != -1, " must be a valid turl")
		self.getxattr_enoent(''.join([get_val("TEST_SRM_READ_VALID"), "_testenoent"]), "user.replicas");
		
	def test_guid_lfn(self):
		v= self.getxattr_valid(get_val("TEST_LFN_READ_VALID"), "user.guid");
		self.assertTrue( len(v) == 36 , "must be a correct len %d"%(len(v)))
		bis = self.getxattr_valid("guid:"+ v, "user.guid"); # get the guid of a guid, must be the same
		self.assertTrue( v == bis , "must be the same guid")
	
