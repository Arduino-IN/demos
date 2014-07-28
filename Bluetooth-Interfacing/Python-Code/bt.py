import bluetooth
bd_addr = "00:12:10:23:10:18" 

target_name = "HC-05"
target_address = None

nearby_devices = bluetooth.discover_devices()

for bdaddr in nearby_devices:
    if target_name == bluetooth.lookup_name( bdaddr ):
        target_address = bdaddr
        break

if target_address is not None:
	print "found target bluetooth device with address ", target_address
	port = 1
	sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	sock.connect((target_address, port))
	while True:
		try:
		    sock.send(str.lower(raw_input()+"\n"))
		except:
			print "Closing connection"
			sock.close()
			break
else:
	print "Could not find target bluetooth device nearby"
