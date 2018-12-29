#!/bin/bash

BuildDir="build"

if [ -d $BuildDir ]; then 
   echo "Removing existing build directory {$BuildDir}"
   sudo rm -rf $BuildDir
fi

echo "Creatiing new build directory {$BuildDir}"
sudo mkdir $BuildDir

sudo chmod -R 766 $BuildDir
g++ -o $BuildDir/XSCRun -I XSCFrameWork/include/ -I XSCFrameWork/thirdparty/ XSCFrameWork/thirdparty/*/*.cpp XSCFrameWork/source/*/*.cxx XSCFrameWork/source/*.cxx