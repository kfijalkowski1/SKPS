# Import Libraries
import math

import matplotlib
import numpy as np

import matplotlib.pyplot as plt

# matplotlib.use('Qt5Agg')
# Create Scatter Plot

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
# Move left y-axis and bottom x-axis to centre, passing through (0,0)
ax.spines['left'].set_position('center')
ax.spines['bottom'].set_position('center')

# Eliminate upper and right axes
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
# ax.spines['bottom'].set_color('none')
# ax.spines['left'].set_color('none')

# Show ticks in the left and lower axes only
ax.xaxis.set_ticks_position('bottom')
ax.yaxis.set_ticks_position('left')
ax.set_xlim([-10, 10])
ax.set_ylim([-10, 10])



import socket
UDP_IP = "127.0.0.1"
UDP_PORT = 5005
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

while True:
    message, addr = sock.recvfrom(1024)  #
    str_mess = message.decode("utf-8")
    data = str_mess.split(" ")
    distance, ankle = float(data[0]),  float(data[1])
    # x = np.sin(i)
    # i += 1
    # y = np.cos(30 * x - 5)
    x, y = math.cos(ankle) * distance, math.sin(ankle) * distance
    plt.scatter(x, y, color="red")

    plt.title("Scatter Plot")

    plt.pause(0.2)

