#!/bin/bash

TOP=$PWD

for i in 01 02 03
do
  cd $TOP/$i
  time ./a.gnu
done
