#!/bin/bash
filename=$1
filenamecsv=${filename%.*}".csv"
filenamepj=${filename%.*}".pjdump"


pj_dump --float-precision=9 $filename  > $filenamecsv
grep "Container\|MyState" $filenamecsv >& $filenamepj
rm $filename
rm $filenamecsv


