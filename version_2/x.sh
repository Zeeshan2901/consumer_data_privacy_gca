
START_JAVA=$(date +%s.%N)			#Capturing start of Java Execution
#Executing JAVA Programs in parallel
#java User1 & (sleep 0.04; java User2)
sleep 2
END_JAVA=$(date +%s.%N)				#Capturing end of Java Execution


START_GC=$(date +%s.%N)				#Capturing start of Garbled Circuit Execution
sleep 5
END_GC=$(date +%s.%N)				#Capturing end of Garbled Circuit Execution 

EXECUTION_TIME_1=`printf "%.5f" $(echo "$END_JAVA - $START_JAVA" | bc)`
EXECUTION_TIME_2=`printf "%.5f" $(echo "$END_GC - $START_GC" | bc)`

TOTAL_TIME= $($EXECUTION_TIME_1 + $EXECUTION_TIME_2)

echo "DATA SHARING Execution Time	: $EXECUTION_TIME_1"
echo "GARBLED CIRCUIT Execution Time	: $EXECUTION_TIME_2"
echo "Total Execution Time		: $TOTAL_TIME"
