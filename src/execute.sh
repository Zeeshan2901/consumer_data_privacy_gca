#!/bin/bash

#To use  from config.properties file
source ./config.properties

PATH_1="$DIR_LOCATION$PARTY_1/$USER1_LIST_FILE"
PATH_2="$DIR_LOCATION$PARTY_2/$USER2_LIST_FILE"

LOC_1="$DIR_LOCATION$PARTY_1/"
LOC_2="$DIR_LOCATION$PARTY_2/" 

#echo "dir_location	: $DIR_LOCATION"
#echo "party_1		: $PARTY_1"
#echo "FIle Location	: $USER2_LIST_FILE"
echo "Final Location	: $PATH_1"
#echo "Final Location	: $PATH_2"

# Code to read and print contents of a file
COUNT=0



input=$PATH_1
while IFS= read -r LINE
do
	COUNT=$((COUNT+1))			#Counter
	INPUT_FILE="$LINE.txt"			#Creating Input File Name from the element of the list file
	ATTRIBUTES=(${LINE//_/ })		#Capturing the attributes from the list 0-> CHRMOSOME 1-> CM_START 2-> No. of GENOTYPES
	CIRCUIT="circuit_${ATTRIBUTES[2]}.txt"
	if [ `expr $COUNT % 1` == 0 ]
	then
		#echo "$LINE"
		#echo "$INPUT_FILE"
		#echo ${ATTRIBUTES[@]}
		#echo "CHROMOSOME :" ${ATTRIBUTES[0]}
		#echo "CM_START   :" ${ATTRIBUTES[1]}
		#echo "Genotypes  :" ${ATTRIBUTES[2]}
		#echo "PORT	 : $PORT"
		#echo "LOC_1	 : $LOC_1"
		#echo "INPUT_FILE : $INPUT_FILE"
		#echo "Circuit	 : $CIRCUIT"
		cd ..
		#echo $PWD
		#ls -lart bin/Frame_Match
		bin/Frame_Match $PARTY_1 $PORT $LOC_1 $INPUT_FILE $CIRCUIT & bin/Frame_Match $PARTY_2 $PORT $LOC_2 $INPUT_FILE $CIRCUIT
		#bin/Frame_Match $PARTY_1 $PORT $LOC_1 $INPUT_FILE $CIRCUIT
		cd src	
	fi
done < "$input"
echo "COUNT : $COUNT"
#Frame_Match

