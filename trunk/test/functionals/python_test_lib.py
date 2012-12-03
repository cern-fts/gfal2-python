#!/usr/bin/python

import sys
import os


def get_val(x):
	r = os.getenv(x)
	if(r == None):
		raise AttributeError(" Test parameter not set in env var : "+ x)
	return r
	

def normalize_url(url):
	res = []
	for i in range(len(url)-1):
		if(url[i] != '/' or url[i+1] != '/'):
			res.append(url[i])
	return ''.join(res)


	
