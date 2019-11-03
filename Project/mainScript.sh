#!/bin/bash
echo "Starting mainScript.sh"

echo "Arguments Received"
a=0
for i; do 
   echo "arg [$a]:" $i
   a=$a+1
done

echo "listing directory"
ls .

echo "Calling buildScript.sh"
. Scripts/buildScript.sh

echo "Calling runScript.sh"
. Scripts/runScript.sh $@