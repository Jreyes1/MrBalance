# -*- coding: utf-8 -*-
"""
Created on Thu Feb 13 14:52:48 2020

@author: jr31
"""
from threading import Thread
import serial
import time
import collections
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import struct
import pandas as pd
import copy


class serialPlot:
    def __init__(self, serialPort = '/dev/ttyUSB0', serialBaud = 38400, plotLength = 100, dataNumBytes = 2,numPlots=3):
        self.port = serialPort
        self.baud = serialBaud
        self.plotMaxLength = plotLength
        self.dataNumBytes = dataNumBytes
        self.numPlots = numPlots
        self.rawData = bytearray(numPlots*dataNumBytes)
        self.dataType = None
        if dataNumBytes ==2:
            self.dataType = 'h' # 2 byte integer
        elif dataNumBytes == 4:
            self.dataType = 'f' # 4 byte float
        self.data = []
        for i in range(numPlots):   # give an array for each type of data and store them in a list
            self.data.append(collections.deque([0] * plotLength, maxlen=plotLength))
        self.isRun = True
        self.isReceiving = False
        self.thread = None
        self.plotTimer = 0
        self.previousTimer = 0

# Open Serial Port 
        try:
            self.serialConnection = serial.Serial(serialPort, serialBaud, timeout=4)
            print('Connected to ' + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
        except:
            print("Failed to connect with " + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
#           
            
    # Start background thread to read in data, pause until data is read
    def readSerialStart(self):
        if self.thread == None:
            self.thread = Thread(target=self.backgroundThread)
            self.thread.start()
            # Block till we start receiving values
            while self.isReceiving != True:
                time.sleep(0.1)
     
     # background thread for retreaving data
    def backgroundThread(self):    # retrieve data
        time.sleep(1.0)  # give some buffer time for retrieving data
        self.serialConnection.reset_input_buffer()
        while (self.isRun):
            self.serialConnection.readinto(self.rawData)
            self.isReceiving = True
            print(self.rawData)
         
         
         
    def getSerialData(self, frame, lines, lineValueText, lineLabel, timeText):
        currentTimer = time.perf_counter()
        self.plotTimer = int((currentTimer - self.previousTimer) * 1000)     # the first reading will be erroneous
        self.previousTimer = currentTimer
        timeText.set_text('Plot Interval = ' + str(self.plotTimer) + 'ms')
        
        if self.numPlots ==1:
            value,  = struct.unpack('f', self.rawData)    # use 'h' for a 2 byte integer
            self.data.append(value)    # we get the latest data point and append it to our array
            lines.set_data(range(self.plotMaxLength), self.data)
            lineValueText.set_text('[' + lineLabel + '] = ' + str(value))
            # self.csvData.append(self.data[-1])
        else:
            privateData = copy.deepcopy(self.rawData[:])    # so that the 3 values in our plots will be synchronized to the same sample time
            for i in range(self.numPlots):
                data = privateData[(i*self.dataNumBytes):(self.dataNumBytes + i*self.dataNumBytes)]
                value,  = struct.unpack(self.dataType, data)
                self.data[i].append(value)    # we get the latest data point and append it to our array
                lines[i].set_data(range(self.plotMaxLength), self.data[i])
                lineValueText[i].set_text('[' + lineLabel[i] + '] = ' + str(value))
            # self.csvData.append([self.data[0][-1], self.data[1][-1], self.data[2][-1]])
 
    def close(self):
        self.isRun = False
        self.thread.join()
        self.serialConnection.close()
        print('Disconnected...')
        # df = pd.DataFrame(self.csvData)
        # df.to_csv('/home/rikisenia/Desktop/data.csv')
        
def main():
    portName = 'COM5'
    baudRate = 38400
    maxPlotLength = 100
    dataNumBytes = 4
    numPlots = 3
    s = serialPlot(portName, baudRate, maxPlotLength, dataNumBytes,numPlots)
    s.readSerialStart()
    
   # plotting starts below
    pltInterval = 50    # Period at which the plot animation updates [ms]
    xmin = 0
    xmax = maxPlotLength
    ymin = -(4)
    ymax = 5
    fig = plt.figure(figsize=(10, 8))
    ax = plt.axes(xlim=(xmin, xmax), ylim=(float(ymin - (ymax - ymin) / 10), float(ymax + (ymax - ymin) / 10)))
    ax.set_title('Arduino Analog Read')
    ax.set_xlabel("time")
    ax.set_ylabel("AnalogRead Value")
    
    if s.numPlots == 1:
        lineLabel = 'Potentiometer Value'
        timeText = ax.text(0.50, 0.95, '', transform=ax.transAxes)
        lines = ax.plot([], [], label=lineLabel)[0]
        lineValueText = ax.text(0.50, 0.90, '', transform=ax.transAxes)
    else:
        lineLabel = ['x','y','z']
        style = ['r-','c-','b-']
        timeText = ax.text(0.70,0.95,'',transform=ax.transAxes)
        lines = []
        lineValueText = []
        for i in range(numPlots):
            lines.append(ax.plot([], [], style[i], label=lineLabel[i])[0])
            lineValueText.append(ax.text(0.70, 0.90-i*0.05, '', transform=ax.transAxes))
        
    anim = animation.FuncAnimation(fig, s.getSerialData, fargs=(lines, lineValueText, lineLabel, timeText), interval=pltInterval)    # fargs has to be a tuple
    print("here3")
    
    plt.legend(loc="upper left")
    plt.show()
 
    s.close()
    
if __name__ == '__main__':
    main()
    
    