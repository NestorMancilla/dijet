#!/bin/bash

# sum trigger luminosities from brilcalc outputs of different IOVs
# exclude-dir is really used to exclude directories
# tuned for jet triggers

lumis=$(grep -r "#HLT_PFJet"${1}"_v" --exclude-dir={*fwd,B2,C,D,E,Fe}|cut -d , -f 6)
#lumis=$(grep -r "#HLT_ZeroBias_" --exclude-dir=*fwd|cut -d , -f 6)

#echo ${lumis}

sum=0
for lum in ${lumis[@]}; do 
     sum=$(echo $sum + $lum | bc)
done
printf "%.3f\n" "$sum"
