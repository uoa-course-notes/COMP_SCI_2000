#!/bin/bash

# Check if a filename is provided as an argument
if [ -z "$1"]; then
	echo "Please provide a test script file name ending in .tst"
	exit 1
fi

# Run the HardwareSimulator.sh script with the provided test script's filename
sh ~/COMP_SCI_2000/tools/HardwareSimulator.sh prac2/part2/"$1"
