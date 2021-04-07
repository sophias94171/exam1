import matplotlib.pyplot as plt

import numpy as np

import serial

import time


Fs = 1000.0;  # sampling rate
Ts = 1.0/Fs; # sampling interval
N = 256
t = np.arange(0, Ts*N,Ts) # time vector; create Fs samples between 0 and 1.0 sec.
n = len(t) # length of the signal

k = np.arange(n)

T = n/Fs

frq = k/T # a vector of frequencies; two sides frequency range

frq = frq[range(int(n/2))] # one side frequency range


serdev = '/dev/ttyACM0'
s = serial.Serial(serdev)
y = np.zeros_like(t) # signal vector; create Fs samples
while(1):
    line=s.readline().decode('utf-8')[:-2]
    if line[0]== "-":
        break
    else:
        print(line)
        idx, value = line.split(",")
        idx = int(idx)
        if idx < N:
            y[idx] = float(value)

Y = np.fft.fft(y)/n*2 # fft computing and normalization
Y = Y[range(int(n/2))] # remove the conjugate frequency parts


fig, ax = plt.subplots(2, 1)


ax[0].plot(t,y)
ax[0].set_xlabel('Time')
ax[0].set_ylabel('Signal y')

ax[1].plot(frq,abs(Y),'r') # plotting the spectrum
ax[1].set_xlabel('Freq (Hz)')
ax[1].set_ylabel('|Y(freq)|')
plt.show()

s.close()