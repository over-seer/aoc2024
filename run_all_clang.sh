#!/bin/bash

TOP=$PWD

for i in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 18 19 20 21
do
  cd $TOP/$i
  time ./a.clang
done
