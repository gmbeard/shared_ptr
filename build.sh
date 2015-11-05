#!/bin/bash

mkdir build
pushd build
g++ \
  -std=c++98 \
  -Wall -O0 -g \
  -I.. -I../submodules/catch/include \
  -o test \
  ../tests/testmain.cpp \
  ../tests/shared_ptr_tests.cpp \
  ../tests/scoped_ptr_tests.cpp
popd

