import os
import sys
import re
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import csv

def make_histogram(path):
    arrays = [[], [], []]

    with open(path) as file:
        for line in file:
            if "Client" in line or "waiting" in line:
                continue
            client_time = re.findall(r"(?<=client )(\d), delivery time: (\d*)", line)
            if len(client_time) == 0:
                continue
            if len(client_time[0]) != 2:
                continue
            arrays[int(client_time[0][0]) - 1].append(int(client_time[0][1]))

    colors = ['red', 'tan', 'lime']
    plt.hist(arrays, 25, histtype='bar', color=colors, label=colors)
    plt.legend(['Client0', 'Client1', 'Client2'])
    plt.show()

values = []
with open("server.txt") as f:
    # content = f.read()
    for row in csv.reader(f, delimiter=","):
        # print(row)
        values.append(int(row[2]))
print(values)

# colors = ['red', 'tan', 'lime']
plt.hist(values[1:], 125, histtype='bar')
# plt.legend(['Client0', 'Client1', 'Client2'])
plt.show()