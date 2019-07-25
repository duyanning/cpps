#!/usr/bin/env bash

if [ $# -lt 2 ] ; then
	echo "Usage: $0 <.sh name> vc"
	exit 1
fi 

#batname="${1%.*}.bat"
#echo $batname

#sed  -e "/#!/d" -e "s/^#/@REM/g" -e "s/gcc.exe/$2.exe/g" -e "s/-c gcc/-c $2/g" -e "s@/dev/null@NUL@g" $1  -e "s/^cp /COPY /g" -e "s/rm/DEL/g" -e 's/\//\\/g'
sed  -e 's/#!.*/@ECHO OFF/g' -e "s/^#/@REM/g" -e "s/gcc.exe/$2.exe/g" -e "s/-c gcc/-c $2/g" -e "s@/dev/null@NUL@g" $1  -e "s/^cp /COPY /g" -e "s/rm/DEL/g" -e 's/\//\\/g'

