## UID: 905694345
(IMPORTANT: Only replace the above numbers with your true UID, do not modify spacing and newlines, otherwise your tarfile might not be created correctly)

## Pipe Up

This is low level c code that connects command line arguments together, same as the | operation. 

## Building

To build the program, navigate to inside the lab1 directory and run 'make'.

## Running

Run the program by using './pipe <arguments>'. To prevent error, you must provide atleast one other argument than pipe. If there is only one other argument, the argument will be executed the same as if it was by itself. No flags should be provided, and every argument is a valid command line argument. 

Example 1: 
>./pipe ls cat

Output:
>Makefile
>pipe
>pipe.c
>README.md
>test_lab1.py

*This preforms the exact same way as if you were to type 'ls | cat' in the command line*

Example 2:
>./pipe ls cat wc

Output:
>6      6      51

*This preforms the exact same way as if you were to type 'ls | cat | wc' in the command line*

## Cleaning up

Run 'make clean' to clean up all files from make. If you used the python test suite then run 'rm -r __pycache__' to remove unnecessary files generated. Pytest will automatically run make clean for you. 