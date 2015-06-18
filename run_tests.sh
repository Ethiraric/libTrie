#!/bin/sh

# The file containing the main function
MAINFILE=./tests/main.c
# The library to link with
LIBRARY=libTrie.a
# The temporary executable file
TEMPEXE=tests.exe


# Runs coverage tests on the library with gcov
# The file tests/main.c is used as the main file and linked with the library
# All gcov output files are moved to the tests folder
# A temporary file tests.exe is created in the folder

# Compile everything with -fprofile-arcs -ftest-coverage
make "CFLAGS=-ggdb3 -fprofile-arcs -ftest-coverage" re
gcc -ggdb3 -fprofile-arcs -ftest-coverage $MAINFILE -l:$LIBRARY  -Iinclude -o $TEMPEXE

# Run the file and run gcov
valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./$TEMPEXE
gcov src/*.gcda

# Remove temporary gcov files and the executable
rm -f *.gcda src/*.gcda
rm -f *.gcno src/*.gcno
rm -f $(basename $MAINFILE).gcov
rm -f $TEMPEXE

# Move output gcov files to tests
mv *.gcov tests/
