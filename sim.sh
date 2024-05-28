#!/bin/bash 

# Check if a filename is provided as an argument 
if [ -z "1"]; then 
    echo "Please provide a test script filename ending in.tst for your .asm file"
    exit 
fi 


# Run the CPU Emulator 
sh ~/COMP_SCI_2000/prac6/tools/CPUEmulator.sh "$1"