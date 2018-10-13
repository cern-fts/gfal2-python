#!/usr/bin/env python

import sys
import gfal2


if(len(sys.argv) < 2):
	print("Usage %s [URL]"%(sys.argv[0]))
	exit(1)

ctxt = gfal2.creat_context()
dirp = ctxt.opendir(sys.argv[1])

while(True):
	(dirent, stat) = dirp.readpp()
	if(dirent is None):
		break;
	print("filename %s, filesize: %d, filerights 0%o"%(dirent.d_name, stat.st_size, stat.st_mode))


	

