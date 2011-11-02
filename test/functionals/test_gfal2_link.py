#!/usr/bin/python

import sys
import gfal2
import errno
import time
import stat

import unittest
from python_test_lib import *

class Testgfal2_link(unittest.TestCase):
	 
	def define_symlink_from_file(self, url):
		return ''.join([url, "_link_", str(time.time())])
		  
	def link_valid(self, url):
		link = self.define_symlink_from_file(url)
		gfal2.symlink(url, link)
		st = gfal2.lstat(link)
		self.assertTrue(stat.S_ISLNK(st.st_mode), "must be a link")
		res = gfal2.readlink(link)
		self.assertTrue( normalize_url(res) == normalize_url(url), "must be the same url %s %s"%(res,url))
		gfal2.unlink(link) # delete the link
		st= gfal2.lstat(url) # original must still exist
		try:
			st= gfal2.lstat(link)
			self.assertTrue( False, "must not reach here, this url must not be a existing one")				
		except gfal2.GError, e:
			self.assertTrue( e.code() == errno.ENOENT, "must not reach here, this url must not be a existing one")			
	
	def test_link_lfn(self):
		self.link_valid(get_val("TEST_LFN_READ_VALID"))
			
	def test_lstat_srm(self):
		pass

	
