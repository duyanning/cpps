#!/usr/bin/env bash

. tests

for i in $DIRS
do
    echo running $i
    cd $i
    ./test.sh | FileCheck test.sh
    cd ..
    echo done $i
done
