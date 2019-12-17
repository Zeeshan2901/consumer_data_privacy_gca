#!/bin/bash

###############To use Variables from config.properties file
source ./config.properties

###############Setting varaiables to be used in the execution
PATH_1="$DIR_LOCATION$PARTY_1/$USER1_LIST_FILE"
PATH_2="$DIR_LOCATION$PARTY_2/$USER2_LIST_FILE"
LOC_1="$DIR_LOCATION$PARTY_1/"
LOC_2="$DIR_LOCATION$PARTY_2/" 

echo "Removing all existing user files from the previous runs"
###############Removing all existing user files from the previous runs
rm $LOC_1*
rm $LOC_2*
rm input/User1/lof/*
rm input/User2/lof/*
>matching_results.txt
echo "Removed"

echo "Compiling JAVA programs"
###############Compiling JAVA programs
javac User1.java
javac User2.java
echo "Compiled"

echo "Executing JAVA Programs in parallel"
###############Executing JAVA Programs in parallel
java User1 & (sleep 0.02; java User2)
echo "Executed"


###############echo "Sleeping for 5 ms so that all previous executions are completed ports are unbinded"
sleep 5;


############### Creating the GC files for each runs iteration by reading inputs from "input/User1/GC_files.txt"


GC_INPUT="input/User1/GC_files.txt"
while IFS= read -r LINE
do	
	cd ..
	pwd
	#echo "line : $LINE"
	OLD_FILE="test/Frame_Match_V2.cpp"			#This is the base program
	GC_FILE="test/Frame_Match_"$LINE".cpp"			#Generated program with const static int runs value
	APPEND_STRING="const static int runs = "$LINE";"	#The string which is added to the file
	#echo "OL : $OLD_FILE"
	#echo "GC : $GC_FILE"
	#echo "AS : $APPEND_STRING"
	echo "$APPEND_STRING" | cat - $OLD_FILE > temp && mv temp $GC_FILE	#Append the string and created a new file
	chmod 777 $GC_FILE
	
	TEST="add_test(Frame_Match_"$LINE")"			#Generating add_test(Frame_Match_XXX) string
	#echo "TT : $TEST"
	echo "$TEST" >> CMakeLists.txt				#Appending the string to CMakeLists.txt file so that we can test and compile it

	cd version_2/

done < "$GC_INPUT"

cd ..
make
cd version_2/

echo "Reading the list of files and executing GC"
############### Code to read and print contents of list file
############### Adding all the list of files from lof/ directory in an array
############### so that we can iterate through the array and process each file in the garbled circuit
cd $LOF_DIR
shopt -s nullglob
LIST_OF_FILES=(*)
#echo ${LIST_OF_FILES[*]}



cd ../../..
pwd
i=0
while [ $i -lt ${#LIST_OF_FILES[@]} ] 
do
	#pwd
	 
	#echo "Line : ${LIST_OF_FILES[$i]}" 					# To print index, ith element
	CIR="$(cut -d'_' -f1 <<<"${LIST_OF_FILES[$i]}")"
	
	USER1_LOFILE=$PARENT$DIR_LOCATION$PARTY_1"/lof/"${LIST_OF_FILES[$i]}
	USER2_LOFILE=$PARENT$DIR_LOCATION$PARTY_2"/lof/"${LIST_OF_FILES[$i]}
	CIRCUIT_FILE=$PARENT$CIRCUIT_DIR$CIR".txt"
	DIR=$PARENT$DIR_LOCATION
	NUM_EXECS=$(< "$DIR_LOCATION$PARTY_1"/lof/"${LIST_OF_FILES[$i]}" wc -l)
	#echo "CF :  $CIRCUIT_FILE"
	#echo "UF :  $USER1_LOFILE"
	#echo "UF :  $USER2_LOFILE"
	#echo "DI :  $DIR"
	
	GC_FILE="bin/Frame_Match_"$NUM_EXECS
	echo "GC $GC_FILE"
	cd ..
	pwd
	#executing the Garbled Circuit Program in parallel
	(sleep 0.10; $GC_FILE $PARTY_1 $PORT $DIR $USER1_LOFILE $CIRCUIT_FILE) & $GC_FILE $PARTY_2 $PORT $DIR $USER2_LOFILE $CIRCUIT_FILE
	#echo "$GC_FILE $PARTY_1 $PORT $DIR $USER1_LOFILE $CIRCUIT_FILE"
	sleep 1;
	cd version_2/

	
	# Increment the i = i + 1 
	i=`expr $i + 1` 
done


echo "Program Completed"

echo "Total Frames	: `wc -l matching_results.txt`"
echo "Matches		: `grep -wc "Match" matching_results.txt`"


