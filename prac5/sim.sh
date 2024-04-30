#!/bin/bash




# Check if a filename is provided as an argument 
if [ -z "1"]; then 
	echo "Please provide a test script filename ending in .tst"
	exit 1
fi 


# Run the CPU emulator 
sh ~/COMP_SCI_2000/prac5/tools/CPUEmulator.sh "$1"
