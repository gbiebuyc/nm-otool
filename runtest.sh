#!/bin/bash


#file=./test_binaries/ppctest
file=./test_binaries/32_exe_hard
nm $file > out1
./ft_nm $file > out2
git diff --no-index out1 out2
rm out1 out2
