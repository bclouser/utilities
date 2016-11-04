#!/usr/bin/env python
import serial
import time

serialdevice = "/dev/ttyUSB1"

# I think i may need to some more here. SOmetimes this doesn't output on the port

port =serial.Serial(port=serialdevice, baudrate=9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, timeout=0.5)

#while 1:
bytes_list = []

port.write('\r')
bytes_list.extend( port.read(32) )
print bytes_list
del bytes_list[:]

port.write('PWR?\r')
bytes_list.extend( port.read(32) )
print bytes_list
del bytes_list[:]

port.write('SOURCE?\r')
bytes_list.extend( port.read(32) )
print bytes_list
del bytes_list[:]

port.write('MSEL?\r')
bytes_list.extend( port.read(32) )
print bytes_list
del bytes_list[:]

port.write('CMODE?\r')
bytes_list.extend( port.read(32) )
print bytes_list
del bytes_list[:]

port.write('PWR OFF\r')
bytes_list.extend( port.read(32) )
print bytes_list
del bytes_list[:]

#for byte in bytes_list:
#	print byte.decode("utf-8") 

