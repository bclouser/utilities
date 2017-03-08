#!/usr/bin/env python

import time
import wiringpi


# use 'GPIO naming'
wiringpi.wiringPiSetupGpio()
 
# set #18 to be a PWM output
wiringpi.pinMode(18, wiringpi.GPIO.PWM_OUTPUT)
 
# set the PWM mode to milliseconds stype
wiringpi.pwmSetMode(wiringpi.GPIO.PWM_MODE_MS)
 
# divide down clock
wiringpi.pwmSetClock(192)
wiringpi.pwmSetRange(2000)
 
delay_period = 0.01

def point(currentVal, gotoVal):
	if currentVal == gotoVal:
		return	

	if currentVal > gotoVal:
		for i in range(0,currentVal-gotoVal):
			wiringpi.pwmWrite(18,currentVal-i)
			time.sleep(delay_period)
	else:
		for i in range(0,gotoVal-currentVal):
			wiringpi.pwmWrite(18,currentVal+i)
			time.sleep(delay_period)


point(150, 56)

