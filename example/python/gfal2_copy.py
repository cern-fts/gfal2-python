#!/usr/bin/env python
import gfal2
import logging
import optparse
import sys

gfal2.set_verbose(gfal2.verbose_level.debug)


def event_callback(event):
	#print event
	print "[%s] %s %s %s" % (event.timestamp, event.domain, event.stage, event.description)

def monitor_callback(src, dst, average, instant, transferred, elapsed):
	print "[%4d] %.2fMB (%.2fKB/s)\r" % (elapsed, transferred / 1048576, average / 1024),
	sys.stdout.flush()

if __name__ == '__main__':
	# Parse arguments
	parser = optparse.OptionParser(usage = 'usage: %prog [options] source destination')
	parser.add_option('-s', '--source-space-token', dest = 'source_space_token',
						help = 'Source space token')
	parser.add_option('-d', '--dest-space-token', dest = 'dest_space_token',
						help = 'Destination space token')
	parser.add_option('-K', '--validate', dest = 'validate', action = 'store_true',
						help = 'Validate source and destination checksum',
						default = False)
	parser.add_option('-c', '--checksum', dest = 'checksum',
						help = 'Checksum (i.e. ADLER32:1234)')
	parser.add_option('-o', '--overwrite', dest = 'overwrite', action = 'store_true',
						help = 'Overwrite destination')

	(options, args) = parser.parse_args()
	if len(args) != 2:
		parser.error('Incorrect number of arguments. Need a source and a destination.')

	source = args[0]
	dest   = args[1]

	print "Source:      %s" % source
	print "Destination: %s" % dest

	# Instantiate gfal2
	ctx = gfal2.creat_context()

	# Set transfer parameters
	params = ctx.transfer_parameters()
	params.event_callback   = event_callback
	params.monitor_callback = monitor_callback

	if options.overwrite:
		params.overwrite = True
		print "Enabled overwrite"

	if options.validate or options.checksum:
		params.checksum_check = True
		print "Enabled checksum check"

	if options.checksum:
		(alg, val) = options.checksum.split(':')
		params.set_user_defined_checksum(alg, val)
		print "User defined checksum: %s:%s" % params.get_user_defined_checksum()

	if options.source_space_token:
		params.src_spacetoken = options.source_space_token
		print "Source space token: %s" % params.src_spacetoken
	if options.dest_space_token:
		params.dst_spacetoken = options.dest_space_token
		print "Destination space token: %s" % params.dst_spacetoken

	# Five minutes timeout
	params.timeout = 300

	# Copy!
	# Keep in mind source and destination can have different protocols,
	# and gfal2 will deal with it
	# (i.e. source can be file:/// and destination gsiftp://)
	try:
		r = ctx.filecopy(params, source, dest)
		print "Copy succeeded!"
	except Exception, e:
		print "Copy failed: %s" % str(e)
		sys.exit(1)

