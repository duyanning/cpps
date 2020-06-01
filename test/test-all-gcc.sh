#!/usr/bin/env bash

# . tests

# for i in $DIRS

for i in */ ;
do
    cd "$i"
    if [ -f "run-gcc.sh" ]
	then
	    echo running $i
    	./run-gcc.sh | FileCheck run-gcc.sh
		echo done $i
	fi
    cd ..
done
