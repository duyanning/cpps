#!/usr/bin/env bash

# . tests

# for i in $DIRS

for i in */ ;
do
    cd $i
    if [ -f "run-clang.sh" ]
	then
	    echo running $i
    	./run-clang.sh | FileCheck run-clang.sh
		echo done $i
	fi
    cd ..
done
