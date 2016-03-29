#!/bin/bash
filename="findlog.txt"

find ./ -type f -name 'logaction' > $filename
echo "">gettime.txt

cat $filename | while read LINE
do
name=$LINE;
newname=${name%/*};
echo ${newname##*/} >>gettime.txt
awk  'END{split($0,a," ");gsub("\\[","",a[1]);gsub("\\]","",a[1]);print a[1]}' $LINE >>gettime.txt
done

