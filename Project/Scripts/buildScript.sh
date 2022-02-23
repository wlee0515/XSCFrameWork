#!/bin/bash

echo "Starting buildScript.sh"

BuildDir="build"

if [ -d $BuildDir ]; then 
   echo "Removing existing build directory {$BuildDir}"
   rm -rf $BuildDir
fi

echo "Creating new build directory {$BuildDir}"
mkdir $BuildDir

echo "Setting directory permission to 757"
chmod -R 757 $BuildDir

echo "Start Compilation"
g++ -pthread -o $BuildDir/XSCRun \
                       -I XSCFrameWork/thirdparty/ \
                       -I XSCFrameWork/include/ \
                       XSCFrameWork/thirdparty/*/*.cpp \
                       XSCFrameWork/source/*/*.cxx \
                       XSCFrameWork/source/*/*/*.cxx \
                       XSCFrameWork/source/*.cxx \
                       -Wall
                       
echo "End Compilation"
