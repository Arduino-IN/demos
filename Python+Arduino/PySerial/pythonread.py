#! /usr/bin/python

import serial
ser = serial.Serial('/dev/ttyACM0',9600)

while True:
	try:
		print(ser.readline())
	except KeyboardInterrupt:
		ser.close()
		print "Received exit command, quitting.."
		break
