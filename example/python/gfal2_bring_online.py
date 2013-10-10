#!/usr/bin/python
# -*- coding: utf-8 -*-
##
# Example : bring online a file with gfal 2.0
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
        # bring_online(surl, pintime, timeout, async)
        (status, token) = ctx.bring_online(surl, 60, 60, False)
        print "Got token %s" % token
        while status == 0:
            status = ctx.bring_online_poll(surl, token)
        print "File brought online!"
    except gfal2.GError, e:
        print "Could not bring the file online:"
        print "\t", e.message
	print "\t Code", e.code
        sys.exit(2)

    try:
        ctx.release(surl, token)
        print "File released!"
    except gfal2.GError, e:
        print "Could not release the file:"
        print "\t", e.message
	print "\t Code", e.code
        sys.exit(3)

    sys.exit(0)

