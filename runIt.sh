#!/bin/bash
echo "Starting RunIt Script"

echo "Arguments Received"
 for i; do 
    echo "arg[$i] :" $i
 done

echo "Changing Directory to Project folder"
cd ./Project

echo "Calling mainScript.sh"
. mainScript.sh $@
