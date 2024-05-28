#!/bin/bash 


# check if a commit message is provided 
if [ -z "$1"]; then 
    echo "Please provide a commit message"
    exit 1
fi 


# Add all changes 
git add .


# Commit changes with the provided message 
git commit -m "$1"


# Push to the remote repositoru Assignment_6
git push origin Assignment_6
