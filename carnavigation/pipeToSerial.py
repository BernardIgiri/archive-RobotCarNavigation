import serial

s = serial.Serial("/dev/ttyUSB0", 115200)

while(True):
	tePipe = open('/home/bernard/carnavigation/datapipe', 'r');
	cmd=tePipe.read();
	s.write(cmd+"\n");
	print cmd+"\n";
	tePipe.close();

