#!/bin/sh
# requires apt-get install libxtst-dev
g++ -std=c++11 -DEMULATOR=0 -DD2KSERVER -DNULL_VALUE=0 -I.. main.cpp ../common/*.cpp ../common/iniparser/src/*.c core/*.cpp -lX11 -lXtst -ods2key
