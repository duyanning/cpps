#!/usr/bin/env bash

# . tests

# for i in $DIRS

for i in */ ;
do
    cd $i
    if [ -f "test.sh" ]
	then
	    echo running $i
    	./test.sh | FileCheck test.sh
		echo done $i
	fi
    cd ..
done
