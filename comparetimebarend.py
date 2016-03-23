import matplotlib.pyplot as plt
import numpy as np
xData = []
x = []
yData = []
y = []
zData = []
z = []
counter = 0
start = 0
end = 0
dur = 0
for line in open("comparetime.txt"):
        counter=counter+1
	newline = line.rstrip('\n')
	line_split = newline.split(':')
	if line_split[0] == 'start time difference':
		start = counter
	elif line_split[0] == 'end time difference':
                end = counter
        elif line_split[0] == 'duration difference':
                dur = counter

counter = 0
for line in open("comparetime.txt"):
        counter=counter+1
        newline = line.rstrip('\n')
        line_split = newline.split(':')
        if counter > start and counter < end:
                xData.append(float(line_split[2]))
		x_split = line_split[1].split(' ')
                x.append(int(x_split[0]))
        elif counter > end and counter < dur:
                yData.append(float(line_split[2]))
        elif counter > dur: 
                zData.append(float(line_split[2]))
N = len(x)
ind = np.arange(N)  # the x locations for the groups
width = 0.35       # the width of the bars
fig, ax = plt.subplots()
rects1 = ax.bar(ind, yData, width, color='r')

# add some text for labels, title and axes ticks
ax.set_ylabel('time (s)')
ax.set_xlabel('processid')
ax.set_title('end time difference')
ax.set_xticks(ind + width)
ax.set_xticklabels(x)



plt.savefig('images/end_difference.png',format='png')
