import os
import sys
import serial
import serial.tools.list_ports
import pandas as pd 
import plotter
import time

# Lists the available serial ports
def availableSerialPorts():
  clist = []
  for p in serial.tools.list_ports.comports():
    clist.append(p.device)
  return clist

# Prints the available serial ports
def printAvailableSerialPorts():
   # Getting the available ports
  print("\n Available serial ports:")
  for port in availableSerialPorts():
    print(" -  "+port)
 
# Calculates the trajectory based on discrete data
class Trajectory:
  def __init__(self, time, altitude):
    self.t = time
    self.h = altitude
    self.sz = len(time)
    self.lastIdx = 1 # used to accelerate the lookup
  
  def altitude(self, t):
    if t <= self.t[0]:
      return self.h[0]
    elif t >= self.t[-1]:
      return self.h[-1]
    else:
      firstIdx = 1
      if t > self.t[self.lastIdx]:
        firstIdx = self.lastIdx
      for i in range(firstIdx,self.sz):
        if t < self.t[i]:
          self.lastIdx = i
          return self.h[i-1]+(self.h[i]-self.h[i-1])*(t-self.t[i-1])/(self.t[i]-self.t[i-1])


def importData(filename):
  df = pd.read_csv(filename, sep="\s+", comment='#', header=None) 
  time     = df[df.columns[0]].to_numpy()
  altitude = df[df.columns[1]].to_numpy()
  return([time,altitude])
  

class BracketsMessageParser:
    """
    Finds messages enclosed by brackets FSB and FSE  
    """
    def __init__(self, FSB, FSE):
      if FSB==FSE:
        raise(ValueError())
      self.FSB = FSB
      self.FSE = FSE
      return

    def parse(self, text):
      # Removing new line characters
      text = text.replace("\n","")
      text = text.replace("\r","")

      # Creating an empty message list
      msgList = []

      while True:
        # Separates the text in three parts: before the field separator, 
        # the field separator and after the field separator. Uses the end
        # field separator.
        beforeFS, fieldseparator, afterFS = text.partition(self.FSE) 
        
        # If the end field separator was found, may be the first part
        # of the text contains a message. Lets take a look...
        if fieldseparator:

          # Setting the new text to be parsed is the text after the field separator.
          text = afterFS

          try:
            # Search for the rightmost index of the begin field separator.
            # If no error occur, a message was found. Add it to the list.
            idxB = beforeFS.rindex(self.FSB)+1
            msgList.append(beforeFS[idxB:])
          except ValueError:
            pass  
        else:
          # If there is no field separator, just exit the loop
          break

      return [text,msgList]


# Detecting the operating system
eol="\n"
isPosix = False # Windows
if os.name == 'posix':
  eol="\n"
  isPosix = True # Linux or Mac

# Getting the number of arguments of the command line
nargs = len(sys.argv)
if nargs != 4:
  print("Usage: python "+sys.argv[0]+" <serial port> <trajectory file> <delay (s)>")
  print(" - serial port: serial port name, e.g., COM3, /dev/ttyS0, etc ")
  print(" - trajectory file: text file with time (s) and altitude (m)")
  print(" - delay: delay (s) to start the simulation")
  exit()
else:
  serialPort     = sys.argv[1]
  trajectoryFile = sys.argv[2]
  try:
    delay = float(sys.argv[3])
  except:
    print("Wrong delay: "+sys.argv[3])
  # Importing data
  try:
    data = importData(trajectoryFile)
    trajectory = Trajectory(data[0],data[1])
    ofilename = os.path.splitext(trajectoryFile)[0]+"-rRocketSimulation.txt"
  except:
    print("Fail to load file "+trajectoryFile+".")
    exit()

  if serialPort not in availableSerialPorts():
    print("Serial port "+serialPort+" unavailable.")
    printAvailableSerialPorts()
    exit()


parser = BracketsMessageParser("<",">")

port=serialPort
baudrate=115200
timeout=0.001
write_timeout=0.05
try:
  ser = serial.Serial(port=port,baudrate=baudrate,timeout=timeout,write_timeout=write_timeout)
  print("Connected to "+port+".")
except:
  print("Could not connect to serial port " + port +". Available ports:")
  printAvailableSerialPorts()
  exit()

buffer = ""
rRocketCurrentState  = ""
rRocketPreviousState = ""
class BreakIt(Exception): pass
try:
  with open(ofilename,"w") as ofile:
      time.sleep(5)
      omsg = "<6,1>"
      ser.write(omsg.encode('UTF-8')) 
      while (True):
        # Getting the inicodeng bytes
        inflow = ser.read(ser.in_waiting or 1)

        # If any byte, adds it to the buffer. Parses the buffer.
        if inflow:
            # If in a posix system (linux or mac), removes return carriage character
            if isPosix:
                inflow = inflow.replace(b'\r\n', b'\n')
            
            inflow = inflow.decode('UTF-8', 'replace') # replaces bad characters with replacement character (U+FFFD)
            
            # Parsing data
            buffer = buffer + inflow
            result = parser.parse(buffer)
            buffer = result[0]
            for msg in result[1]:
                msgSplit=msg.split(",")
                if msgSplit[0]=='1':
                  t=int(msgSplit[1])*1e-3-delay # reading the time and converting it from ms to s
                  h=int(100.0*trajectory.altitude(t)) # converts the altitude from m to cm
                  ans = "<7,"+str(h)+">"
                  ser.write(ans.encode('UTF-8'))
                elif msgSplit[0]=='2':
                  rRocketPreviousState = rRocketCurrentState
                  rRocketCurrentState = msgSplit[4]
                  ofile.write(msg+eol)
                  print(msg)
                  if rRocketCurrentState != rRocketPreviousState:
                    if rRocketCurrentState == "L" and rRocketPreviousState == "P":
                      raise BreakIt
except:
   ser.close()
   print("Disconnected")

print("Plotting...")
plotter.plot(ofilename,trajectoryFile)