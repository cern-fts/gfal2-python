#!/usr/bin/python

import sys
import os


def get_val(x):
	r = os.getenv(x)
	if(r == None):
		raise AttributeError(" Test parameter not set in env var : "+ x)
	return r
	
