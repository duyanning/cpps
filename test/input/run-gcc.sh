#!/usr/bin/env bash

cpps --clear -c gcc hello.cpp <<EOF
 tom
EOF
# CHECK: hello, tom

