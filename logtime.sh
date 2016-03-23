#!/bin/bash
filename=$1
filetime=${filename%.*}"_logtime.xml"
filetimed=${filename%.*}"_logtimedur.xml"
echo "start time:">$filetime
awk -F'[:" "]' '!d[$2]++&&NR>=9 {
gsub("]","",$4);
printf("process=\"%s\" start_time=\"%s\"\n",$2,$4)}
' $filename >> $filetime
echo "end time:">>$filetime
awk -F'[:" "]' ' NR>=9{
gsub("]","",$4);
a[$2]=$4;
}END{for(i=1;i<asorti(a,b);i++)
{
printf("process=\"%s\" end_time=\"%s\"\n",b[i],a[b[i]])}}' $filename >>$filetime

LastRow=`cat $filetime | wc -l`
LastRow=$[ LastRow - 2] 
Row=`expr $LastRow / 2`
echo "duration time:" >$filetimed
Row=$[ Row + 1]
CurRow=2
while [ $CurRow -le $Row ]
do
process1=`awk -F'"' 'NR=='$CurRow' {print $2}' $filetime`
time1=`awk -F'"' 'NR=='$CurRow' {print $4}' $filetime`
process2=`awk -F'"' 'NR=='$CurRow+$Row' {print $2}' $filetime` 
time2=`awk -F'"' 'NR=='$CurRow+$Row' {print $4}' $filetime`
if [ "$process1" == "$process2" ];then
echo -n "process=\"$process1\" duration=\"">>$filetimed
echo $time2 - $time1 | bc>>$filetimed
fi
((CurRow++))
done 


  
