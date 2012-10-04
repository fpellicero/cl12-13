#!/bin/bash
eval "make"
cl="./cl < $1 >sol"
eval $cl
kompare="kompare s$1 sol"
eval $kompare
clean="rm sol"
eval $clean
