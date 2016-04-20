# storage-simulator - a tool for simulating the storage system  using the simgrid API and the trace file  got by cluefs.

## Overview
'storage-simulator' is a simulator which simulating the function of the storage system ,using the data got for LSST(Large Synoptic Survey Telescope) by clusfs ,it is fulfilled according to the simgrid ,most of the api is provided by simgrid ,and we use the trace replay function to fulfill our target.

## Motivation
The main goal of developing the utility is to using the data got from LSST to simulator the storage system ,changing the development file to see which parameter affects the  performance of the storage system most and then we can optimize the storage system of LSST.

### How to use

		1  Download and install simgrid version 3.12  as:http://simgrid.gforge.inria.fr/simgrid/3.12/doc/install.html
		2  Use configure.sh to produce configuration files(filename_deployment.xml,filename_action_trace.txt,storage_content.txt,filename_csvtime.txt) with the command:./configure.sh filename.csv
		3  Compile the program with the command "make"
		4  Run the simulator with the command:./command_aciton.sh 30_100_120_200
### Explanation of command and the files:
*  command_action.sh is a shell script with the command to run the simulator which really works with some action with the trace file:lsst-demo-v10.1-ubuntu-v14.04-cluefs.csv 
   command_sleep.sh is a shell script with the command to run the simulator with sleeping function for the trace file:lsst-demo-v10.1-ubuntu-v14.04-cluefs.csv
* The format of directory 30_100_120_200 is Write-bandwidth(slow)_Read-bandwidth(slow)_Write-bandwidth(fast)_Read-bandwidth(fast).In the directory,there  is the platform.xml which correspond to the parameters.
* When you want to run the program , use the command:
 e.g: ./command_action.sh 30_100_120_200 32
32 is that you want to read 32 kb from the fast disk.
Then you will find the result: logaction file in the directory 30_100_120_200.You can check the log and find something useful.
* in the command_action.sh file ,there is the following command
:./extracttrace --log=replay.thres:verbose --cfg=surf/precision:1e-9 $1/one_host_one_disk.xml lsst-demo-v10.1-ubuntu-v14.04-cluefs_deployment.xml lsst-demo-v10.1-ubuntu-v14.04-cluefs_action_trace.txt action >& $1/logaction

* command_action_randr-psync.sh,command_action_randrw-psync.sh,command_action_read_psync.sh,command_sleep_randr-psync.sh,command_sleep_randrw-psync.sh,command_sleep_read_psync.sh are the command for running the simulator to simulate the trace for fio aciton.

* drawlineplots.py drawlineplots.sh is used for drawing some plot to compare how the application will work with different size store in the fast disk .

*tracetransfer.sh is to do some transfer fome the trace we got from the simulator to the trace needed by framesoc.

 * platform.xml:In order to run any simulation, SimGrid must be provided with three things: something to run (i.e., your code), a description of the platform on which you want to simulate your application and lastly information about the deployment process.http://simgrid.gforge.inria.fr/simgrid/3.12/doc/platform.html
 
 * deployment.xml:When using SimGrid, you basically need your user code, a platform description, and something allowing to map your (simulated) process on your (simulated) platform. This is what deployment file is all about.So deployment file just consists of saying which process runs where and which arguments it should take as input.http://simgrid.gforge.inria.fr/simgrid/3.12/doc/deployment.html
 
 * action_trace.txt:It describes the actions that each process executes.It is needed when simgrid replay the trace.
    The format of this file is :
    pid,action,filepath,duration,openid(open,read,write,flush,release,creat,readdir),position(read,write),bytereq(read,write)
     
 * storage_content.txt:It describes the initial state of the storage disk.
    The format of this file is:filepath,filesize
    This file is used in the platform.xml .

 * action(sleep):It is a parameter to decide which action you want your simulator to do.
* When you want to draw pictures of the executing time of different platform .You can run ./gettime.sh to get the file gettime.txt which includes the platform information and the executing time ,then you run: python script.py to get the picture in the images directory,thirdly run: display images/plot1.png to show the picture.To make it clear the steps are:
		1:./gettime.sh
		2:python script.py
		3:display images/plot1.png
* When you want to get the time of all the read,write,compute action during the executing,you can run rwc.sh to get te rwc.log which includes the total time of read,write and compute action.
* When you want to compare the start and end time of the origin time in the trace file and the time you got in the log file after executing of every process to make sure your simulator works well.you can run:
		 ./comparetime.sh tracefilename_cvstime.xml platformfolder/logaction_logtime.xml 
to get the comparetime.txt file which includes the difference of the start and end time of every process.
It will produce file comparetime.txt which includes all the time difference of starttime ,endtime and duration time.Then you can use python script to draw some picture to make it clear.
  for example: ./comparetime.sh cluefs-centos7-lsst_stack_demo-11.0_cvstime.xml 30_100_120_200/logaction_logtime.xml  
               python comparetimebarstart.py,python comparetimebarend.py,python comparetimebardur.py
* 
* some explanation of the extracttrace.c,I fulfill the simread function with the first 4096 block store in the fast disk and others (the data)store on the slow disk,and the simwrite function is that all the data are write to the slow disk

### verify the simulator
* 1.When you use the command :./configure.sh filename.csv
   It will produce the file filename_csvtime.xml which shows the start and end time of every process according to the .csv trace file.

* 2.After you run the simulator.You can use the script logtime.sh to parsing your logaction or logsleep with the command:./logtime.sh logaction.It will produce the file logaction_logtime.xml in the same directory of logaction.Then you can compare the file filename_csvtime.xml with logaction_logtime.xml in detail and decide whether the result is right.

### What is on going 
Now is on going with changing the configuration of the storage.xml such as the parameter of the disk ,to see the change of time and then draw it on the plot as a curve to describe the time changes with the change of the disk parameter.
