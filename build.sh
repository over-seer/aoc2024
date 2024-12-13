#!/bin/bash

TOP=$PWD

for i in 01 02 03 04 05 06 07 08 09 10 11 12 13
do
    echo $i
    #cd $TOP/$i
    g++ $i/main.cpp -I. -O3 -std=c++23 -Wall -Wextra -Wnrvo -o   $i/a.gnu &
    clang++-18 $i/main.cpp -I. -O3 -std=c++23 -Wall -Wextra -o   $i/a.clang &
done
