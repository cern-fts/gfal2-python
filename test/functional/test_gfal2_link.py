#!/usr/bin/python

import sys
import gfal2
import errno
import time
import stat

import unittest
from python_test_lib import *

class Testgfal2_link(unittest.TestCase):

        def setUp(self):
            self.context = gfal2.creat_context()
	 
	def define_symlink_from_file(self, url):
		return ''.join([url, "_link_", str(time.time())])
		  
	def link_valid(self, url):
		link = self.define_symlink_from_file(url)
                self.context.symlink(url, link)
                st = self.context.lstat(link)
		self.assertTrue(stat.S_ISLNK(st.st_mode), "must be a link")
                res = self.context.readlink(link)
		self.assertTrue( normalize_url(res) == normalize_url(url), "must be the same url %s %s"%(res,url))
                self.context.unlink(link) # delete the link
                st= self.context.lstat(url) # original must still exist
		try:
                        st= self.context.lstat(link)
			self.assertTrue( False, "must not reach here, this url must not be a existing one")				
		except gfal2.GError, e:
			self.assertTrue( e.code() == errno.ENOENT, "must not reach here, this url must not be a existing one")			
	
	def test_lstat_srm(self):
		pass

	
