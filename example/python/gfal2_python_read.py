#!/usr/bin/python
# -*- coding: utf-8 -*-
##
# Example : open/read/close a file with gfal 2.0
#

import sys
import os
import gfal2

max_size=10000000 # maximum size to read

## main func
if __name__ == '__main__':
	# comment for usage
	if(len(sys.argv) < 2):
		print "\nUsage\t %s [gfal_url] \n"%(sys.argv[0])
		print "    Example: %s lfn:/grid/dteam/myfile "%(sys.argv[0])
		print "             %s srm://myserver.com/myhome/myfile \n"%(sys.argv[0])
		os._exit(1)
		
	# open the file
	f = gfal2.file(sys.argv[1], 'r')
	# read the max_size first bytes.
	content = f.read(max_size)
	print content
	
	# no close needed, done automatically with the destruction of the file handle
