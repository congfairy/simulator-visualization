#!/bin/bash
file1=$1
file2=$2
file3=`echo ${file1/.xml/dur.xml}`
file4=`echo ${file2/.xml/dur.xml}`
CurRow=2
LastRow=`cat $file1 | wc -l`
LastRow=$[ LastRow - 2]
Row=`expr $LastRow / 2`
echo "start time difference:">comparetime.txt
Row=$[ Row + 1]
while [ $CurRow -le $Row ]
do
process1=`awk -F'"' 'NR=='$CurRow' {print $2}' $file1`
time1=`awk -F'"' 'NR=='$CurRow' {print $4}' $file1`
process2=`awk -F'"' 'NR=='$CurRow' {print $2}' $file2`
time2=`awk -F'"' 'NR=='$CurRow' {print $4}' $file2`
if [ "$process1" == "$process2" ];then
echo -n "process is:$process1 difference is:">>comparetime.txt 
echo $time2 - $time1 | bc>>comparetime.txt
fi
((CurRow++))
done

echo "end time difference:">>comparetime.txt
CurRow=$[ CurRow + 1]
LastRow=$[ LastRow + 2]
while [ $CurRow -le $LastRow ]
do
process1=`awk -F'"' 'NR=='$CurRow' {print $2}' $file1`
time1=`awk -F'"' 'NR=='$CurRow' {print $4}' $file1`
process2=`awk -F'"' 'NR=='$CurRow' {print $2}' $file2`
time2=`awk -F'"' 'NR=='$CurRow' {print $4}' $file2`
if [ "$process1" == "$process2" ];then
echo -n "process is:$process1 difference is:">>comparetime.txt 
echo $time2 - $time1 | bc>>comparetime.txt
fi
((CurRow++))
done


echo "duration difference:">>comparetime.txt
LastRow=`cat $file3 | wc -l`
CurRow=2
while [ $CurRow -le $LastRow ]
do
process1=`awk -F'"' 'NR=='$CurRow' {print $2}' $file3`
time1=`awk -F'"' 'NR=='$CurRow' {print $4}' $file3`
process2=`awk -F'"' 'NR=='$CurRow' {print $2}' $file4`
time2=`awk -F'"' 'NR=='$CurRow' {print $4}' $file4`
if [ "$process1" == "$process2" ];then
echo -n "process is:$process1 difference is:">>comparetime.txt 
echo $time2 - $time1 | bc>>comparetime.txt
fi
((CurRow++))
done

