#!/usr/bin/python

import sys
import gfal2
import errno

import unittest
from python_test_lib import *


class Testgfal2Qos():
    def setUp(self):
        self.context = gfal2.creat_context()
        s_cred = gfal2.cred_new("BEARER", "eyJraWQiOiJyc2ExIiwiYWxnIjoiUlMyNTYifQ.eyJzdWIiOiJmZWE1ZTZlMi0wYjlmLTQwZjUtYjE5OC00YmI3YWU0YjIzNGEiLCJpc3MiOiJodHRwczpcL1wvaWFtLmV4dHJlbWUtZGF0YWNsb3VkLmV1XC8iLCJleHAiOjE1MzEzMTE2NzcsImlhdCI6MTUzMTMwODA3NywianRpIjoiMTNmNTEyM2QtZjY5My00Yjg4LWI3MjMtNjc2MzFlMmQ4NWM0In0.DaG2P2NJmyi9TD6dJ3fM2AK6p_NcOQE8PRBT3-PvJioWi6uY2JL8cctvk37KIQEZD3VekUqGXucUNGsAQ2y8bmSE7F7zGfc8n6dwq7kymrIRA1sf8e_mcWKXpsVhb4o_rmoQNsMHqq8BYPLXpY_9JJY_XK1tkJOjAAX0-erJLaw")
        gfal2.cred_set(self.context, "dcache-xdc.desy.de", s_cred)
        #print self.context
        #for method_name in dir(self.context):
        #    print method_name
        print self.context.qos_check_classes("https://dcache-xdc.desy.de:6443", "dataobject")
        print self.context.check_file_qos("https://dcache-xdc.desy.de:6443/Users/paul/test-1")
        print self.context.check_available_qos_transitions("https://dcache-xdc.desy.de:6443/cdmi_capabilities/dataobject/disk")
        print self.context.check_target_qos("https://dcache-xdc.desy.de:6443/Users/paul/test-1")
        print self.context.change_object_qos("https://dcache-xdc.desy.de:6443/Users/paul/test-1", "/cdmi_capabilities/dataobject/tape")

a = Testgfal2Qos()
a.setUp()