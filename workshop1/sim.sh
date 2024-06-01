#!/bin/bash 


# Check if a filename has been provided as an argument 
if [ -z "1"]; then 
    echo "Please provide a filename ending in .jack for the compiler to recognize."
    exit 
fi

# Run the Jack compiler 
sh ~/COMP_SCI_2000/tools/JackCompiler.sh "$1"
