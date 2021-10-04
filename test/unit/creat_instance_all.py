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

    def test_multiple_cred(self):
        context = gfal2.creat_context()
        cred_x509 = context.cred_new("X509_CERT", "/tmp/proxy")
        cred_bearer = context.cred_new("BEARER", "mytoken")

        context.cred_set("https://example.host/path/", cred_x509)
        context.cred_set("https://example.host/path/file", cred_bearer)

        (value, prefix) = context.cred_get("BEARER", "https://example.host/path/file")
        self.assertEqual((value, prefix), ("mytoken", "https://example.host/path/file"))

        (value, prefix) = context.cred_get("X509_CERT", "https://example.host/path/file")
        self.assertEqual((value, prefix), ("/tmp/proxy", "https://example.host/path/"))

    def test_cred_delete(self):
        context = gfal2.creat_context()
        cred = context.cred_new("BEARER", "mytoken")
        cred2 = context.cred_new("BEARER", "mytoken2")
        cred3 = context.cred_new("BEARER", "mytoken3")

        context.cred_set("https://example.host/path", cred)
        context.cred_set("https://example.host/path/", cred2)
        context.cred_set("https://example.host/path/file", cred3)

        (value, prefix) = context.cred_get("BEARER", "https://example.host/path/file.tpc")
        self.assertEqual((value, prefix), ("mytoken2", "https://example.host/path/"))
        
        context.cred_del("BEARER", "https://example.host/path/")
        (value, prefix) = context.cred_get("BEARER", "https://example.host/path/file.tpc")
        self.assertEqual((value, prefix), ("mytoken", "https://example.host/path"))
        
        context.cred_del("BEARER", "https://example.host/path")
        (value, prefix) = context.cred_get("BEARER", "https://example.host/path/file.tpc")
        self.assertEqual((value, prefix), ("", ""))


if __name__ == '__main__':
    unittest.main()
