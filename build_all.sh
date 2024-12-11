#!/bin/bash

TOP=$PWD

mkdir build_gnu
cd build_gnu
export CXX=g++
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j12
cd $TOP

mkdir build_clang
cd build_clang
export CXX=clang++-18
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j12
cd $TOP

for i in 01 02 03 04 05 06 07 08 09 10 11
do
  cp -rf build_gnu/${i} ${i}/a.gnu
  cp -rf build_clang/${i} ${i}/a.clang
done
