import matplotlib
import matplotlib.pyplot as plt 
import pandas as pd 
import os

def importData(filename):
  df = pd.read_csv(filename, sep=",", comment='#', header=None) 
  time     = df[df.columns[1]].to_numpy()
  altitude = df[df.columns[2]].to_numpy()
  speed    = df[df.columns[3]].to_numpy()
  state    = df[df.columns[4]]
  
  return [time*1E-3,altitude,speed,state] 

def extractEvents(data):
  time = []
  altitude = []
  speed = []
  state = []
  for i in range(1,len(data[0])):
    if ( data[3][i] != data[3][i-1] ):
      time.append(data[0][i-1])
      altitude.append(data[1][i-1])
      speed.append(data[2][i-1])
      state.append(data[3][i])
      #print(data[0][i-1],data[1][i-1],data[2][i-1],data[3][i])
  return [time,altitude,speed,state] 

def plot(filename, title): 
  data = importData(filename)
  figfilename = os.path.splitext(filename)[0]+".pdf"

  events = extractEvents(data)

  font = {'size'   : 8}

  matplotlib.rc('font', **font)
    
  # plot lines 
  plt.plot(data[0], data[1], linestyle='--', marker='+', label = "Altitude (m)") 
  plt.plot(data[0], data[2], linestyle='--', marker='x', label = "Velocidade (m/s)") 
  plt.plot(events[0],events[1],'d', markersize=5,color="black")
  plt.plot(events[0],events[2],'d', markersize=5,color="black")

  # zip joins
  for x,y,label in zip(events[0],events[1],events[3]):
      plt.annotate(label+" ("+str(int(y))+" m)", # this is the text
                  (x,y), # these are the coordinates to position the label
                  textcoords="offset points", # how to position the text
                  xytext=(20,5), # distance from text to points (x,y)
                  ha='center') # horizontal alignment can be left, right or center
  for x,y,label in zip(events[0],events[2],events[3]):
      plt.annotate(label+" ("+str(int(y))+" m/s)", # this is the text
                  (x,y), # these are the coordinates to position the label
                  textcoords="offset points", # how to position the text
                  xytext=(20,-15), # distance from text to points (x,y)
                  ha='center') # horizontal alignment can be left, right or center
  plt.legend() 
  plt.grid()
  plt.xlabel("t (s)")
  plt.title(title)
  plt.savefig(figfilename)  
  #plt.show()

if __name__=="__main__":
  plot("launch-09-rRocketSimulation.txt", "Lançamento 09")