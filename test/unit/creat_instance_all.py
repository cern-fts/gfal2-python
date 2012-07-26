#!/usr/bin/python

import sys
import gfal2
import time

import unittest

class unit_test_instance(unittest.TestCase):


        def test_creat_and_delete_instance(self):
            gfal2.set_verbose(gfal2.verbose_level.debug)
            context = gfal2.creat_context()
            del context

        def test_creat_and_delete_params(self):
            gfal2.set_verbose(gfal2.verbose_level.debug)
            context = gfal2.creat_context()
             t = context.transfer_params()
             self.assertTrue(t.timeout > 0 )
             t.timeout = 10
             self.assertTrue(t.timeout == 10 )
             self.assertTrue(self.check_checksum == False)
             t.check_checksum = True
             self.assertTrue(self.check_checksum == True)
             self.assertTrue(t.src_spacetoken == "")
             t.src_spacetoken = "TOKENDTEAM"
             self.assertTrue(t.src_spacetoken == "TOKENDTEAM")

        def test_creat_and copy(self):
            gfal2.set_verbose(gfal2.verbose_level.debug)
            c = gfal2.creat_context()
            c.timeout=200
            self.assertTrue(t.timeout == 200 )
            d = c.copy()
            self.assertTrue(d.timeout == 200 )
            c.timeout = 20
            del c
            self.assertTrue(d.timeout == 200 )



if __name__ == '__main__':
    unittest.main()
