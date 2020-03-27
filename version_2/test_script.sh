#!/bin/bash

FILE_DIR="test_files/case3/"
EXTENSION=".txt"
for (( i = 1; i <= 14; i++ ))      ### Outer for loop ###
do
	U1="$FILE_DIR$i$EXTENSION"

	for (( j = ($i +1) ; j <= 15; j++ )) ### Inner for loop ###
	do
		U2="$FILE_DIR$j$EXTENSION"
		echo -n "( $U1 , $U2 ) "
		./execute.sh $U1 $U2
		
	done
done
