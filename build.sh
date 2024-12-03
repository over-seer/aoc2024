#!/bin/bash

TOP=$PWD

for i in 01 02 03
do
    echo $i
    cd $TOP/$i
    g++ main.cpp -O3 -std=c++23 -Wall -Wextra -Wnrvo -o a.gnu
    clang++-18 main.cpp -O3 -std=c++23 -Wall -Wextra -o a.clang
done
