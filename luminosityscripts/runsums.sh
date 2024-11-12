#!/bin/bash



for i in 40 60 80 140 200 260 320 400 450 500; do 
#    echo $i
    bash sums.sh $i
done


for i in 40 60 80 140 200 260 320 400 450 500; do 
#    echo Fwd$i
    bash sums.sh Fwd$i
done
