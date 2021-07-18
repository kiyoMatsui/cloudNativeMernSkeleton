#!/bin/bash

valgrind --tool=memcheck --leak-check=yes ./build/src/beastserver

###If you want to debug a program with GDB when using the Memcheck tool, start Valgrind like this:

#valgrind --vgdb=yes --vgdb-error=0 prog
###In another shell, start GDB:

#gdb ./prog
###Then give the following command to GDB:

#(gdb) target remote | vgdb

#for emacs gdb run M-x gud-gdb; select program, then type break and the name of the function where to stop.
