#!/bin/bash

BuildDir="build"

if [ -d $BuildDir ]; then 
  else
    mkdir $BuildDir
fi

g++ -o build/XSCRun -I XSCFrameWork/include/ -I XSCFrameWork/thirdparty/ XSCFrameWork/source/*.cxx XSCFrameWork/thirdparty/*.cpp