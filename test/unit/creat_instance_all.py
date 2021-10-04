#!/usr/bin/env python

import gfal2

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
        self.assertGreater(t.timeout, 0)
        t.timeout = 10
        self.assertEqual(t.timeout, 10)

        self.assertEqual(t.checksum_check, False)
        t.checksum_check = True
        self.assertEqual(t.checksum_check, True)

        self.assertEqual(t.src_spacetoken, "")
        t.src_spacetoken = "TOKENDTEAM"
        self.assertEqual(t.src_spacetoken, "TOKENDTEAM")


if __name__ == '__main__':
    unittest.main()
