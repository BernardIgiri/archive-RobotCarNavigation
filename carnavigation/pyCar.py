import serial
#screen size 320x480

maxX = float(480)
maxY = float(550)
s = serial.Serial("/dev/ttyUSB0", 115200)

def transformPoint(x,y):
	scaledx = (float(x)/maxX)*255 
	scaledy = (float(y)/maxY)*255 
	(left,right) = (capValue(scaledy + scaledx), capValue(scaledy - scaledx))
	return (left,right)



def capValue(val):
	if val > 255:
		val = 255
	elif val < -255:
		val = -255
	return val 


def handleNewPoint(command):
	try:
		(x,y) = command.split(",")
		(left,right) = transformPoint(x,y)
		s.write("L " + str(int(left)) + "\n")
		s.write("R " + str(int(right)) + "\n")
		print "L " + str(int(left)) + " R " + str(int(right))
	except:
		print "Error occurred. Ignore and move on"

while(True):
	tePipe = open('/home/bernard/carnavigation/pipe', 'r')
	cmd=tePipe.read();
	handleNewPoint(cmd);
	tePipe.close()

