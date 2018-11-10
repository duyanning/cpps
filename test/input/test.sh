#!/usr/bin/env bash

cpps --clear hello.cpp <<EOF
 tom
EOF
# CHECK: hello, tom

