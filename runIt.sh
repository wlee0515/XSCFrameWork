#!/bin/bash
echo "Starting RunIt Script"

echo "Arguments Received"
a=0
for i; do 
   echo "arg [$a]:" $i
   a=$a+1
done

echo "Changing Directory to Project folder"
cd ./Project

echo "Calling mainScript.sh"
. mainScript.sh $@