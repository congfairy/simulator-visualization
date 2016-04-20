import matplotlib.pyplot as plt
import numpy as np
import pylab as pl
xData = []
yData = []
zData = []
x = []
yalready = 0
zalready = 0
counter = 0
for line in open("gettime.txt"):
        newline = line.rstrip('\n')
        if counter>=1 and counter%2==0:
		if yalready==1:
		   yappend=newline
		   yalready=0
		elif zalready==1:
		   zappend=newline
		   zalready=0
                else:
                   xData.append(newline)
	elif counter>=1 and counter%2!=0:
              if newline=="0":
	        yalready = 1
	      elif newline=="20480":
		zalready = 1
	      else:
		x.append(newline)
        counter=counter+1
for index in range(len(x)):
   yData.append(yappend)
   zData.append(zappend)
# add some text for labels, title and axes ticks
plot1 = plt.plot(x,xData,'ro',label="different head size")
plot2 = plt.plot(x,yData,label="all in the slow disk")
plot3 = plt.plot(x,zData,label="all in the fast disk")
plt.legend(bbox_to_anchor=(1.12, 1.13), loc=1, borderaxespad=0.)
plt.ylabel('executing time (s)')
plt.xlabel('Head size(MB)')
plt.title('executing time of different head size',horizontalalignment='right',fontsize=15)
plt.savefig('images/runningtime.png',format='png')
