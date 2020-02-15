#!/bin/bash


file=./test_binaries/ppctest
nm $file > out1
./ft_nm $file > out2
git diff --no-index out1 out2
rm out1 out2
