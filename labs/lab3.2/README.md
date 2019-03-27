# Lab 3.2
## base64.c
**base64.c** is a program that lets you encode and decode text files based on the *Base64* algorithm.
This program also handles the **SIGINT** and **SIGINFO** signals. The signal handler makes it so when a signal is received, the program stops for
about 2 seconds to display the percentage progress of the current encoding or decoding.
## Compiling and running
To compile, simply type `make` and the binary file **base64** be created. To run the program, type `./base64 --<encode || decode> <file.txt>`.
This repository already contains 3 text files that can be used for testing.  
Running `./base64 --encode <file.txt>` will create a new text file named
**encoded.txt** with the encoded text. Running `./base64 --decode <file.txt>` will create a new text file named **decoded.txt** with the
decoded text.  
To clean up your directory from output files, type `make clean`.
