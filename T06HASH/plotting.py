import matplotlib.pyplot as plt
import numpy as np
import sys
import os

try:
# inputs
    numOfLists = int (sys.argv[1])
    inFileName = sys.argv[2]

# data init
    x = np.arange (0, numOfLists)
    y = np.loadtxt (inFileName, delimiter='\n', dtype=np.int)

    name = os.path.splitext (inFileName)[0]

# plotting
    plt.plot (x, y, 'o', markersize = 2.0)
    
    plt.title  (os.path.basename (name))
    plt.xlabel ("listId")
    plt.ylabel ("elements")
    plt.text   (max (x) * 0.7, max (y), 'Dispercion = ' + str (int (np.var (y))))
    plt.ylim   ([0, max (y) * 1.2])

    plt.savefig (name + '.png')

except:
    print ("plotting error(" "\n")