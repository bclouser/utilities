#!/usr/bin/env python 

""" 
A simple TCP client 
""" 

import socket 

host = '10.90.231.20' 
port = 5003 
size = 1024 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.connect((host,port)) 
s.send('MAP: 10000 123971 133971 250643 264136') 



while(True):
	data = s.recv(size)	
	if data:
		print data
	



s.close();
#data = s.recv(size) 
#s.close() 
#print 'Received:', data
