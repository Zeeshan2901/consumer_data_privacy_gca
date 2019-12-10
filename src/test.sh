#!/bin/bash
START=$(date +%s.%N)

sleep 0.01;

END=$(date +%s.%N)

echo "START	: $START"
echo "END	: $END"


DURATION=$(echo "$END - $START" | bc)
EXECUTION_TIME=`printf "%.5f seconds" $(echo "$END - $START" | bc)`

echo "Script Execution Time: $EXECUTION_TIME"



shopt -s nullglob
array=(*)
array2=(*.java)
array3=(input/*)

echo "Array 1 : ${array[*]}"
echo "Array 2 :${array2[*]}"
echo "Array 3 :${array3[*]}"
