#!/usr/bin/env bash

cpps --clear -c gcc show-args.cpp a b 'c d'
# CHECK: argc: 4
# CHECK-NEXT: arg[0]: show-args.cpp
# CHECK-NEXT: arg[1]: a
# CHECK-NEXT: arg[2]: b
# CHECK-NEXT: arg[3]: c d
