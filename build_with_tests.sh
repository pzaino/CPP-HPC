#!/bin/bash

mkdir -p ./build
cd ./build
cmake -DENABLE_TESTING=ON ..
make
