#!/bin/bash
#To use  from config.properties file
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
>matching_results.txt
echo "Removed"

echo "Compiling JAVA programs"
#Compiling JAVA programs
javac User1.java
javac User2.java
echo "Compiled"

echo "Executing JAVA Programs in parallel"
START_JAVA=$(date +%s.%N)			#Capturing start of Java Execution
#Executing JAVA Programs in parallel
java User1 & (sleep 0.04; java User2)
END_JAVA=$(date +%s.%N)				#Capturing end of Java Execution
echo "Executed"

echo "Sleeping for 1 second so that all previous executions are completed ports are unbinded"
sleep 1;

echo "Reading the list of files and executing GC"
# Code to read and print contents of list file
COUNT=0
input=$PATH_1
START_GC=$(date +%s.%N)				#Capturing start of Garbled Circuit Execution
while IFS= read -r LINE
do
	COUNT=$((COUNT+1))			#Counter to keep count of all the frames
	INPUT_FILE="$LINE.txt"			#Creating Input File Name from the element of the list file
	ATTRIBUTES=(${LINE//_/ })		#Capturing the attributes from the list 0-> CHRMOSOME 1-> CM_START 2-> No. of GENOTYPES
	CIRCUIT="circuit_${ATTRIBUTES[2]}.txt"	#Creating the Circuit File for Garbled Circuit Program
	if [ `expr $COUNT % 1` == 0 ]
	then
		
		cd ..				#moving to the directory for Garbled Circuit program execution
		#executing the Garbled Circuit Program in parallel
		(sleep 0.05; bin/Frame_Match $PARTY_1 $PORT $LOC_1 $INPUT_FILE $CIRCUIT) & bin/Frame_Match $PARTY_2 $PORT $LOC_2 $INPUT_FILE $CIRCUIT
		cd src				#moving back to the execution directory
	fi
done < "$input"
END_GC=$(date +%s.%N)				#Capturing end of Garbled Circuit Execution 
echo "Program Completed"
echo "Total Frames	: $COUNT"
echo "Matches		: `grep -wc "Match" matching_results.txt`"

EXECUTION_TIME_1=`printf "%.5f seconds" $(echo "$END_JAVA - $START_JAVA" | bc)`
EXECUTION_TIME_2=`printf "%.5f seconds" $(echo "$END_GC - $START_GC" | bc)`

echo "DATA SHARING Execution Time	: $EXECUTION_TIME_1"
echo "GARBLED CIRCUIT Execution Time	: $EXECUTION_TIME_2"
