#!/bin/bash

FILE_DIR="test_files/case3/"
FILE_DIR1="test_files/case4/"
TXT=".txt"
CSV=".csv"
for (( i = 1; i <= 16; i++ ))      ### Outer for loop ###
do
	U1="$FILE_DIR$i$TXT"
	
	#U1=$FILE_DIR"79.csv"
	#U2=$FILE_DIR"91.txt"

	#for (( j = ($i+1) ; j <= 54 ; j++ )) ### Inner for loop ###
	for (( j = 90 ; j <= 94; j++ ))
	do
		U2="$FILE_DIR1$j$TXT"
		echo -n "( $i, $U1 , $U2 ) "
		echo ""
		./execute.sh $U1 $U2
		
	done
done
