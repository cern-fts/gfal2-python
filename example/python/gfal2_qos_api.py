#!/usr/bin/python

import gfal2
import optparse
import sys
from urlparse import urlparse

if __name__ == "__main__":
    # Override epilog parser to allow newlines
    optparse.OptionParser.format_epilog = lambda self, formatter: self.epilog
    helptext = """
Supported actions:
    - list-classes
    - list-transitions
    - get-qos
    - get-target-qos
    - change-qos <target_qos>

Example:
    [list-classes]:     https://cdmi-server.cern.ch:8443/
    [list-transitions]: https://cdmi-server.cern.ch:8443/cdmi_capabilities/dataobject/class_name
    [get-qos]:          https://cdmi-server.cern.ch:8443/path/to/file
    [get-target-qos]:   https://cdmi-server.cern.ch:8443/path/to/file
    [change-qos]:       https://cdmi-server.cern.ch:8443/path/to/file <target-qos>
"""
    # Parse arguments
    parser = optparse.OptionParser(usage = "usage: %prog <action> <url> <token> [<target_qos>]",
                                   epilog = helptext)
    (options, args) = parser.parse_args()

    if len(args) < 3:
        parser.error("Incorrect number of arguments!")

    action = args[0]
    url = args[1]
    token = args[2]

    if action not in ["list-classes", "list-transitions", "get-qos",
                      "get-target-qos", "change-qos"]:
        parser.error("Unrecognized action: %s" % action)

    if action == "change-qos":
        if len(args) < 4:
            parser.error("Missing <target-qos> argument!")
        target_qos = args[3]

    # Create Gfal2 handler
    context = gfal2.creat_context()
    cred = gfal2.cred_new("BEARER", token)
    host = '{uri.scheme}://{uri.netloc}/'.format(uri=urlparse(url))
    gfal2.cred_set(context, host, cred)

    if action == "list-classes":
        print "Dataobject: %s" % context.qos_check_classes(host, "dataobject")
        print "Container: %s" % context.qos_check_classes(host, "container")
    elif action == "list-transitions":
        print context.check_available_qos_transitions(url)
    elif action == "get-qos":
        print context.check_file_qos(url)
    elif action == "get-target-qos":
        print context.check_target_qos(url)
    elif action == "change-qos":
        print context.change_object_qos(url, target_qos)
    else:
        # Should not reach here
        print "Unrecognized action"
        exit(1)
