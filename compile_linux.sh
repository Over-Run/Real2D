#!/bin/bash

. ./src/compile_sources.sh

target=${sources[*]}
i=0

mkdir -p x64/Linux/

for var in ${sources[*]}
do
    target[i]=src/$var
    i+=1
done

g++ ${target[*]} \
$1 \
-Iinclude -Llib/x64 -std=c++17 \
-lglad2 -lglfw3 -ldl -lX11 -lpthread \
-o x64/Linux/Real2D.out
