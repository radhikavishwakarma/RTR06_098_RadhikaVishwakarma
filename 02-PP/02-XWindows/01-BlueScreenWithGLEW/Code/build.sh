#!/bin/bash
g++ -c -o OGL.o OGL.cpp
g++ -o OGL OGL.o -lX11 -lGL
./OGL