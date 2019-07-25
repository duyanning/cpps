#!/usr/bin/env bash

# 注意：匹配windows路径中的\在minised中用\\就好，在正常sed中得用\\\\
cat main.inc | sed -e "/Note: including file:[ \t]\+C:\\\\Program Files/d" -e "/Note: including file:/w bigsed.txt" -e "/Note: including file:/d"




