#!/bin/bash


if [ -n "$1" ] ; then
	file="$1"
else
	file=./test_binaries/64_exe_medium
fi
nm $file > out1
./ft_nm $file > out2
git diff --no-index out1 out2
rm out1 out2
