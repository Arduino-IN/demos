from pyfirmata import Arduino, util
import sys

board = Arduino('/dev/ttyACM0')

try:
	#board.digital[13].write(0)
	#board.digital[13].write(1)
	it = util.Iterator(board)
	it.start()
	board.analog[0].enable_reporting()
	board.analog[0].read()
except KeyboardInterrupt:
	print "Closing..."
	sys.exit(1)