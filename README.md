# libTrie
This is a simple Trie library developped in C

It is designed as a key-value container, with null terminated strings as keys

The value is of generic type void *

## Compiling the library
Run :

    git clone https://github.com/Ethiraric/libTrie.git
    make

The library is compiled as ./libTrie.a

## Running tests with the library

Run :

    ./run_tests.sh

It will compile and link the library with flags to enable the use of gcov

Then it will create an executable with the file tests/main.c (may be changed
in the file run_tests.sh) and execute it

It will delete temporary or unnecessary files and move gcov output to tests/

## Linking with the library

Compile your program as follow :

    gcc [files] -Lpath/to/library -lTrie -Ipath/to/includes [flags]

Or add `-Lpath/to/library -lTrie` to the LD_FLAGS of your Makefile and
`-Ipath/to/includes` to your CFLAGS

## Bugs and Suggestions

Feel free to report anything you think useful to florian.sabourin@epitech.eu