#!/bin/bash

BuildDir="build"

if [ -d $BuildDir ]; then 
   echo "Removing existing build directory {$BuildDir}"
   sudo rm -rf $BuildDir
fi

echo "Creatiing new build directory {$BuildDir}"
sudo mkdir $BuildDir

echo "Setting directory permission to 757"
sudo chmod -R 757 $BuildDir

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
