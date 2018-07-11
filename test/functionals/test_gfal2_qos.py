#!/usr/bin/python

import sys
import gfal2
import errno

import unittest
from python_test_lib import *


class Testgfal2Qos():
    def setUp(self):
        self.context = gfal2.creat_context()
        s_cred = gfal2.cred_new("BEARER", "eyJraWQiOiJyc2ExIiwiYWxnIjoiUlMyNTYifQ.eyJzdWIiOiJmZWE1ZTZlMi0wYjlmLTQwZjUtYjE5OC00YmI3YWU0YjIzNGEiLCJpc3MiOiJodHRwczpcL1wvaWFtLmV4dHJlbWUtZGF0YWNsb3VkLmV1XC8iLCJleHAiOjE1MzEzMDE0MjcsImlhdCI6MTUzMTI5NzgyNywianRpIjoiMzU0NWRiYzUtNmRjNy00OTk0LWE0MTAtMjA3NmJiMzBmNGMwIn0.BeTGtzDpAZ9_06ii8YraBr7I6XgvlpwVI9fStAkgy79yxQ9K6YXFry6affAqy6Mb7G-t5e0XuQiTPk2fMf_ieM5xfothSGIITwFo8gRIWTBooT6k1YpIf4dmyfOyTYmVdjh27MVeDPHtHK17Ob-58U01fVwKN8tRIuu0GG17kTQ")
        gfal2.cred_set(self.context, "dcache-xdc.desy.de", s_cred)
        #print self.context
        #for method_name in dir(self.context):
        #    print method_name
        print self.context.qos_check_classes("https://dcache-xdc.desy.de:6443", "dataobject")

a = Testgfal2Qos()
a.setUp()