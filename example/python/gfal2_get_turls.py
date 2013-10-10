#!/usr/bin/python
# -*- coding: utf-8 -*-
##
# Example : get list of turls/replicas with gfal 2.0
#

import gfal2
import sys

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "Usage: %s [surl]" % sys.argv[0]
        sys.exit(1)

    surl = sys.argv[1]

    ctx = gfal2.creat_context()
    try:
	replicas = ctx.getxattr(surl, 'user.replicas')
	print replicas
    except gfal2.GError, e:
        print "Could not get the replicas:"
        print "\t", e.message
	print "\t Code", e.code
        sys.exit(2)

    sys.exit(0)

