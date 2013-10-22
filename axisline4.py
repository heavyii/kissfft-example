#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import linecache
import math
def rawData(x):
	dataList = []
	i = 0
	count = linecache.getlines("dump-raw-3.txt")
	for t in x:
		data = count[i].split('=')[4].split('_')[0]
		dataList.append(int(data) )
		i = i + 1
	return dataList

def labviewFftData(x):
	dataList = []
	i = 0
	count = linecache.getlines("01_Real_Part.txt")
	for t in x:
		if i < len(count):
			dataList.append(np.float(count[i]) )
		else:
			dataList.append(np.float(0) )
		i = i + 1
	#print dataList
	return dataList

def fftData(x):
	dataList = []
	i = 0
	count = linecache.getlines("test-output.txt")
	for t in x:
		if i < len(count):
			data = count[i].split(',')[0].split('(')[1]
			#data = count[i].split(',')[1].split(')')[0]
			dataList.append(np.float(data) )
		else:
			dataList.append(np.float(0) )
		i = i + 1
	#print dataList
	return dataList

def freq(x):
	dataList = []
	lenx = 1.0*len(x)/2
	for t in x:
		dataList.append(np.float(t/lenx))
	return dataList

	
def getTargetData(x):
	maxData = 0
	minData = 0
	maxPos = 0
	minPos = 0
	i = 0
	for data in x[0:len(x)/2]:
		if maxData < data:
			maxData = data
			maxPos = i
		if minData > data:
			minData = data
			minPos = i
		i = i + 1
	print (np.float(maxPos), np.float(minPos))
	return math.sqrt(maxPos * maxPos + minPos * minPos)
	
x = np.arange(0, 2047, 1)
y = rawData(x)
z = fftData(x)
#z = freq(z)

fp = 214000 
frx = x*fp/len(x)


labviewFFT = labviewFftData(x)

tdata = getTargetData(z)
print 'target data = %12f' % tdata
plt.figure(figsize=(8,4))
plt.plot(x,y,label="$rawData(x)$",color="red",linewidth=2)
plt.plot(x,z,label="$fftData(x)$",color="blue",linewidth=2)
plt.plot(x,labviewFFT,label="$labviewFFT(x)$",color="green",linewidth=2)
plt.xlabel("time/Frequency(Hz)")
plt.ylabel("volt/Amplitude")
plt.title("FFT")
plt.ylim(-2500, 2500)
plt.legend()
plt.show()
