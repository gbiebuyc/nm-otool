#!/bin/bash

if [ -n "$1" ] ; then
	files="$1"
else
	files=./test_binaries/*
fi

for f in $files
do
	echo "Testing: $f"
	nm $f > out1 2>/dev/null
	./ft_nm $f > out2 2>/dev/null
	git diff --no-index --exit-code out1 out2 || break
	# diff out1 out2 || break
done
rm out1 out2
