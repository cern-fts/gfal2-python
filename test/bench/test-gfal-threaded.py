




import threading

import time

import gfal2
import gfal
import posix
import sys


        
class ThreadClass(threading.Thread):

	def __init__(self, func):
		self.func = func
		threading.Thread.__init__(self)
	
	def run(self):
		self.func()
		

def gfal2_read(url):
	f= gfal2.file(url,'r')
	a= f.read(500)
	print a


def gfal1_read(url):
	f = gfal.gfal_open(url, posix.O_RDONLY)
	a = gfal.gfal_read(f, 500)
	gfal.gfal_close(f)
	print a[1]



if __name__ == "__main__":
	t = []
	r = range(int(sys.argv[3]))
	init_time = time.time()
	for i in r:
		exe = lambda : (globals()[sys.argv[2]](sys.argv[1]))
		t.append(ThreadClass( exe ))
	
	for i in r:
		t[i].start()
		
	for i in r:
		t[i].join()
	
	finish_time = time.time()
	
	print "exec time : %d"%(finish_time-init_time)
			
