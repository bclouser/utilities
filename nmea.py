#!/usr/bin/env python
import socket
import serial
import time

serialdevice = "/dev/tty.usbserial-FTGYEXIC"

serialdevice = "/dev/ttyUSB0"

# I think i may need to some more here. SOmetimes this doesn't output on the port

port =serial.Serial(port=serialdevice, baudrate=4800, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, timeout=5)

longitude = 0
latitude = 0

gga='$GPGGA,123519,4807.%03d,N,01131.%03d,E,1,08,0.9,545.4,M,46.9,M,,*47'% (latitude, longitude)

rmc='$GPRMC,235952,A,4807.%03d0,N,01131.%03d0,E,,,210899,004.0,W*74'%(latitude, longitude)




#while 1:
#    gga='$GPGGA,123519,4807.%03d,N,01131.%03d,E,1,08,0.9,545.4,M,46.9,M,,*47'% (latitude, longitude)
#    print gga
#    port.write(gga + '\r\n')
#    latitude+=1
#    longitude+=1
#    time.sleep(1)


speed = 30.0  #mph

ftInGpsSecond = 95.0 # this is really rough. Google Number of ft in GPS degree, minute, second

timeTodelay = ftInGpsSecond/((speed * 5280)/3600)

print "Time to delay: "
print timeTodelay

#starting values
degrees = 0
minutes = 0  # a minute is roughly a mile... Very rough
seconds = 0  # a second is 80 - 100ft

#we don't care about milliseconds

while 1:
	lat='%02d%02d.%02d00'%(degrees, minutes, seconds)
	lon='%02d%02d.%02d00'%(degrees, minutes, seconds)
	rmc='$GPRMC,235952,A,%s,N,%s,E,,,210899,004.0,W*74'%(lat, lon)
   
	print "lat = " + lat + "lon = " + lon 
	print rmc

	port.write(rmc + '\r\n')

	seconds += 1

	if seconds >= 60:
		minutes += 1
		seconds = 0

		if minutes >= 60:
			degrees += 1
			minutes = 0


	time.sleep( timeTodelay )

