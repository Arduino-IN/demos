#! /usr/bin/python

import serial
ser = serial.Serial('/dev/ttyACM0',9600)

while True:
	try:
		n = input("Please enter the number of times to blink the LED\n")
		ser.write(str(chr(n)))
	except KeyboardInterrupt:
		ser.close()
		print "Received exit command, quitting.."
		break
	except NameError:
		print "Please enter a number, not a character\n"
		pass
