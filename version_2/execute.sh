#!/bin/bash

#To use Variables from config.properties file
source ./config.properties

#Setting varaiables to be used in the execution
PATH_1="$DIR_LOCATION$PARTY_1/$USER1_LIST_FILE"
PATH_2="$DIR_LOCATION$PARTY_2/$USER2_LIST_FILE"
LOC_1="$DIR_LOCATION$PARTY_1/"
LOC_2="$DIR_LOCATION$PARTY_2/" 

echo "Removing all existing user files from the previous runs"
#Removing all existing user files from the previous runs
rm $LOC_1*
rm $LOC_2*
rm input/User1/lof/*
rm input/User2/lof/*
>matching_results.txt
echo "Removed"

echo "Compiling JAVA programs"
#Compiling JAVA programs
javac User1.java
javac User2.java
echo "Compiled"

echo "Executing JAVA Programs in parallel"
#Executing JAVA Programs in parallel
java User1 & (sleep 0.02; java User2)
echo "Executed"


echo "Sleeping for 0.01 ms so that all previous executions are completed ports are unbinded"
sleep 5;


echo "Reading the list of files and executing GC"
# Code to read and print contents of list file

cd $LOF_DIR
shopt -s nullglob
LIST_OF_FILES=(*)
echo ${LIST_OF_FILES[*]}

cd ../../..
pwd
i=0
while [ $i -lt ${#LIST_OF_FILES[@]} ] 
do
	# To print index, ith element 
	echo ${LIST_OF_FILES[$i]} 

	USER1_LOFILE=$PARENT$DIR_LOCATION$PARTY_1"/lof/"${LIST_OF_FILES[$i]}
	USER2_LOFILE=$PARENT$DIR_LOCATION$PARTY_2"/lof/"${LIST_OF_FILES[$i]}
	CIRCUIT_FILE=$PARENT$CIRCUIT_DIR${LIST_OF_FILES[$i]}
	DIR=$PARENT$DIR_LOCATION
	#echo "CF :  $CIRCUIT_FILE"
	#echo "UF :  $USER1_LOFILE"
	#echo "UF :  $USER2_LOFILE"
	#echo "DI :  $DIR"

	cd ..
	#executing the Garbled Circuit Program in parallel
	(sleep 0.10; bin/Frame_Match_V2 $PARTY_1 $PORT $DIR $USER1_LOFILE $CIRCUIT_FILE) & bin/Frame_Match_V2 $PARTY_2 $PORT $DIR $USER2_LOFILE $CIRCUIT_FILE
	sleep 1;
	cd version_2/

	
	# Increment the i = i + 1 
	i=`expr $i + 1` 
done


echo "Program Completed"

echo "Total Frames	: `wc -l matching_results.txt`"
echo "Matches		: `grep -wc "Match" matching_results.txt`"


