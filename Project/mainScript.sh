#!/bin/bash
echo "Starting mainScript.sh"

echo "Arguments Received"
for i; do 
   index=i
   echo "arg[$index] :" $i
done

echo "listing directory"
ls .

echo "Calling buildScript.sh"
. Scripts/buildScript.sh

echo "Calling runScript.sh"
. Scripts/runScript.sh $@
