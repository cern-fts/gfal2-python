#!/usr/bin/python
# -*- coding: utf-8 -*-
##
# Example : bring online a set of files with gfal2
#
import errno
import gfal2
import sys
import time


def evaluate_errors(errors, surls, polling):
    n_terminal = 0
    for surl, error in zip(surls, errors):
        if error:
            if error.code != errno.EAGAIN:
                print "%s => FAILED: %s" % (surl, error.message)
                n_terminal += 1
            else:
                print "%s QUEUED"
        elif not polling:
            print "%s QUEUED" % surl
        else:
           n_terminal += 1
           print "%s READY" % surl
    return n_terminal


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "Usage: %s [surl1] [surl2] [...]" % sys.argv[0]
        sys.exit(1)

    surls = sys.argv[1:]

    ctx = gfal2.creat_context()
    
    print "Bring online"
    print "============"
    (errors, token) = ctx.bring_online(surls, 60, 60, True)
    print "Got token %s" % token
    
    n_terminal = evaluate_errors(errors, surls, polling=False)
    sleep_time = 1
    while n_terminal != len(surls):
        time.sleep(sleep_time)
        sleep_time = min(sleep_time * 2, 600)
        print
        print "Polling"
        print "======="
        errors = ctx.bring_online_poll(surls, token)
        n_terminal = evaluate_errors(errors, surls, polling=True)

    print
    print "Release"
    print "======="
    errors = ctx.release(surls, token)
    for surl, error in zip(surls, errors):
        if error:
            print "%s => RELEASE FAILED: %s" % (surl, error.message)
        else:
            print "%s RELEASED" % surl
    sys.exit(0)

