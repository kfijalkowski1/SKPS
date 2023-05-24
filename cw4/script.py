import os
import sys
import re
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt


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


paths = ["src/zad5_1.log", "src/zad5_2.log"]

for path in paths:
    make_histogram(path)