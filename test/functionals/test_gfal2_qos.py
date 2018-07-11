#!/usr/bin/python

import sys
import gfal2
import errno

import unittest
from python_test_lib import *


class Testgfal2Qos():
    def setUp(self):
        self.context = gfal2.creat_context()
        s_cred = gfal2.cred_new("BEARER", "eyJraWQiOiJyc2ExIiwiYWxnIjoiUlMyNTYifQ.eyJzdWIiOiJmZWE1ZTZlMi0wYjlmLTQwZjUtYjE5OC00YmI3YWU0YjIzNGEiLCJpc3MiOiJodHRwczpcL1wvaWFtLmV4dHJlbWUtZGF0YWNsb3VkLmV1XC8iLCJleHAiOjE1MzEzMDU1MTAsImlhdCI6MTUzMTMwMTkxMCwianRpIjoiZmJjMWE0MWQtNmU2NC00ZTAyLWI3ZjctZGYzODk0Y2VjM2M0In0.WiuBFQHceiSlO1s2OE0vTWStzRMvwpVuxFi0wpINToQ82-dyJF8RVaqODX2Onn-X76UsfKOlaLcdLVmSIzaRI5fnuneWMKdtafHtuA1Takh__bs9iaeOOCgLStMn2DXFMQkGmP-ip4VjMY5kmlSt7P7sKntwkXR35__5-sVQA2g")
        gfal2.cred_set(self.context, "dcache-xdc.desy.de", s_cred)
        #print self.context
        #for method_name in dir(self.context):
        #    print method_name
        print self.context.qos_check_classes("https://dcache-xdc.desy.de:6443", "dataobject")
        #print self.context.check_file_qos("https://dcache-xdc.desy.de:6443/Users/paul/test-1")
        print self.context.check_available_qos_transitions("https://dcache-xdc.desy.de:6443/cdmi_capabilities/dataobject/disk")
        #print self.context.check_target_qos("https://dcache-xdc.desy.de:6443/Users/paul/test-1")
        print self.context.change_object_qos("https://dcache-xdc.desy.de:6443/Users/paul/test-1", "/cdmi_capabilities/dataobject/tape")

a = Testgfal2Qos()
a.setUp()