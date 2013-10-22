#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import linecache

def rawData(x):
	dataList = []
	i = 0
	count = linecache.getlines("dump-raw-3.txt")
	for t in x:
		data = count[i].split('=')[4].split('_')[0]
		dataList.append(int(data))
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
			dataList.append(np.float(data) )
		else:
			dataList.append(np.float(0) )
		i = i + 1
	#print dataList
	return dataList

def unpackComplex(data):
	realList = []
	imageList = []
	for x in data:
		real,image = x
		realList.append(real)
		imageList.append(image)
	return (realList, imageList)

#to frx
PI = 3.1415926535898
fp = 214000 

x = np.linspace(0, 2047, 2047)
y = 300 * np.sin(x/34 - 0.5) + 2000
z = np.fft.fft(y).real/len(y)

frx = x*fp/len(x)

plt.figure(figsize=(8,4))
plt.plot(x,y,label="$rawData(x)$",color="red",linewidth=2)
plt.plot(frx,z,label="$fftData(x)$",color="yellow",linewidth=2)
#plt.plot(x,labviewFFT,label="$labviewFFT(x)$",color="green",linewidth=2)
plt.xlabel("time/Frequency(Hz)")
plt.ylabel("volt/Amplitude")
plt.title("FFT")
plt.ylim(-2500, 2500)
plt.legend()
plt.show()
