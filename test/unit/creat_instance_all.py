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
            t = context.transfer_parameters()
            self.assertTrue(t.timeout > 0 )
            t.timeout = 10
            self.assertTrue(t.timeout == 10 )
            self.assertTrue(t.checksum_check == False)
            t.checksum_check = True
            self.assertTrue(t.checksum_check == True)
            self.assertTrue(t.src_spacetoken == "")
            t.src_spacetoken = "TOKENDTEAM"
            self.assertTrue(t.src_spacetoken == "TOKENDTEAM")



if __name__ == '__main__':
    unittest.main()
