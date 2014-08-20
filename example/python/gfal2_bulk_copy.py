#!/usr/bin/python
# -*- coding: utf-8 -*-
##
# Example : copy a set of files in one go
#
import gfal2
import optparse
import shlex
import sys

def event_callback(event):
    print "[%s] %s %s %s" % (event.timestamp, event.domain, event.stage, event.description)


def monitor_callback(src, dst, average, instant, transferred, elapsed):
    print "[%4d] %.2fMB (%.2fKB/s)\r" % (elapsed, transferred / 1048576, average / 1024),
    sys.stdout.flush()


if __name__ == '__main__':
    # Parse arguments
    parser = optparse.OptionParser(usage = 'usage: %prog [options] file')
    parser.add_option('-K', '--validate', dest = 'validate', action = 'store_true',
                        help = 'Validate source and destination checksum',
                        default = False)
    parser.add_option('-o', '--overwrite', dest = 'overwrite', action = 'store_true',
                        help = 'Overwrite destination')

    (options, args) = parser.parse_args()
    if len(args) != 1:
        parser.error('Incorrect number of arguments. Need a file containing the list of sources and destinations.')
    
    sources = []
    destinations = []
    for line in open(args[0]).readlines():
        l = shlex.split(line)
        if l:
            sources.append(l[0])
            destinations.append(l[1])
            
    print "Found %d pairs" % len(sources)
       
    # Instantiate gfal2
    ctx = gfal2.creat_context()

    # Set transfer parameters
    params = ctx.transfer_parameters()
    params.event_callback   = event_callback
    params.monitor_callback = monitor_callback
    
    if options.overwrite:
        params.overwrite = True
        print "Enabled overwrite"

    if options.validate:
        params.checksum_check = True
        print "Enabled checksum check"
        
    # Copy!
    # In this case, an exception will be thrown if the whole process fails
    # If any transfer fail, the method will return a list of GError objects, one per file
    # being None if that file succeeded
    try:
        errors = ctx.filecopy(params, sources, destinations)
        if not errors:
            print "Copy succeeded!"
        else:
            for i in range(len(errors)):
                e = errors[i]
                src = sources[i]
                dst = destinations[i]
                if e:
                    print "%s => %s failed [%d] %s" % (src, dst, e.code, e.message)
                else:
                    print "%s => %s succeeded!" % (src, dst)
    except Exception, e:
        print "Copy failed: %s" % str(e)
        sys.exit(1)

