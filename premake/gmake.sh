#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
  # Do something under Mac OS X platform
  ./premake5_osx gmake --file=premake5.lua
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
  # Do something under Linux platform
  ./premake5_linux gmake --file=premake5.lua
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
  # Do something under Windows NT platform
  echo "windows? don't use this"
fi

