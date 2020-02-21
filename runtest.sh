#!/bin/bash

exit_script () {
	rm out1 out2
	exit
}

assert_equal () {
	echo "Testing:" $1
	nm $1 > out1 2>/dev/null
	./ft_nm $1 > out2 2>/dev/null
	git diff --no-index --exit-code out1 out2 || exit_script
	echo "Testing: otool" $1
	otool -t $1 > out1 2>/dev/null
	./ft_otool $1 > out2 2>/dev/null
	git diff --no-index --exit-code out1 out2 || exit_script
}

make
if [ -n "$1" ] ; then
	assert_equal "$1"
	exit_script
fi
files=./test_binaries/*
for f in $files; do assert_equal $f; done
assert_equal ""
assert_equal "I don't exist ./test_binaries/a"
assert_equal "$files"
echo "All tests passed."
exit_script
