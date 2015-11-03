#!/bin/bash

mkdir build
pushd build
g++ -std=c++98 -Wall -O0 -g -I.. -o test ../tests/main.cpp
popd

