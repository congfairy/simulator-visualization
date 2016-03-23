import matplotlib.pyplot as plt
import numpy as np
xData = []
x = []

counter = 0
for line in open("gettime.txt"):
        newline = line.rstrip('\n')
        if counter>=1 and counter%2==0:
              xData.append(newline)
	elif counter>=1 and counter%2!=0:
              x.append(newline)
        counter=counter+1
print x
print xData
N = len(x)
ind = np.arange(N)  # the x locations for the groups
width = 0.35       # the width of the bars

fig, ax = plt.subplots()
rects1 = ax.bar(ind, xData, width, color='r')

# add some text for labels, title and axes ticks
ax.set_ylabel('executing time (s)')
ax.set_xlabel('RW bandwith(MB/s)')
ax.set_title('executing time of different platform')
ax.set_xticks(ind + width)
ax.set_xticklabels(x)
def autolabel(rects):
    # attach some text labels
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()/2., 1.05*height,
                float(height),
                ha='center', va='bottom')

autolabel(rects1)
plt.savefig('images/executingtime.png',format='png')
