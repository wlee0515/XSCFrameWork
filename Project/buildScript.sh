#!/bin/bash

BuildDir="build"

if [ -d $BuildDir ]; then 
   echo "Removing existing build directory {$BuildDir}"
   sudo rm -r $LinkDir
fi

echo "Creatiing new build directory {$BuildDir}"
sudo mkdir $BuildDir

g++ -o build/XSCRun -I XSCFrameWork/include/ -I XSCFrameWork/thirdparty/ XSCFrameWork/source/*.cxx XSCFrameWork/thirdparty/*.cpp