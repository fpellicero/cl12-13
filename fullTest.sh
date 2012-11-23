#!/bin/bash
eval "make"
for i in jp*; do
    eval "./cl execute < $i >sol"
    if ! diff sol s$i >/dev/null ; then
	echo $i failed; 
    fi
    eval "rm sol"
done
