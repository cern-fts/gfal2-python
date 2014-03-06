#!/usr/bin/python

import sys
import gfal2


if(len(sys.argv) < 2):
	print("Usage %s [URL]"%(sys.argv[0]))
	exit(1)

ctxt = gfal2.creat_context()
r = ctxt.listdir(sys.argv[1])

map(lambda y: sys.stdout.write(("%s\n")%(y)), r)

