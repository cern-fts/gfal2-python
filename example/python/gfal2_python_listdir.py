#!/usr/bin/python
# -*- coding: utf-8 -*-
##
# Example : list the content of a directory with gfal 2.0
#

import sys
import os
import gfal2


## main func
if __name__ == '__main__':
	# comment for usage
	if(len(sys.argv) < 2):
		print "\nUsage\t %s [gfal_folder] \n"%(sys.argv[0])
		print "    Example: %s lfn:/grid/dteam/ "%(sys.argv[0])
		print "             %s srm://myserver.com/myhome/ \n"%(sys.argv[0])
		os._exit(1)

	ctx = gfal2.creat_context()		
	my_dir = ctx.listdir(sys.argv[1])
	for d in my_dir:
		print d
	
	# no closedir needed, done automatically 
