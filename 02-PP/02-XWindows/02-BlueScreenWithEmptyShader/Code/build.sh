#!/bin/bash
gcc -c -o OGL.o OGL.cpp
gcc -o OGL OGL.o -lX11 -lGL
./OGL