#! /bin/bash  
varvalues=../assets/scripts/values
cd ./../../compilation_debug/
status=0
while read line 
do 
	if [ $status -eq 0 ] ; then
		echo -e parameters: "$line" 
		./projet $line
	fi
	status=$?
done < $varvalues
