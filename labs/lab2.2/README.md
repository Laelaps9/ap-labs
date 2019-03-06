# Lab 2.2
## logger.c
**logger.c** is a library that contains the functions used for printing each type of message.
* INFO: Info. messages are printed in white, bold color.
* WARNING: Warnings are printed in a dim blue color.
* ERROR: Errors are printed in red.
* PANIC: Panics are printed in black text color but the background is changed to pink.  
**logger.h** is the header file of the logger.c functions
## testLogger.c
**testLogger.c** has a main in which one of each type of messages is printed, just to showcase the colors.
## Compiling and running
By typing `make` in terminal, you will be compiling testLogger.c and its dependency *the logger.c library* and outputting it into a binary file *testLogger*. The you can type `./testLogger` to get the messages in **testLogger.c** with their respective colors.  
`make clean` can be used to delete the testLogger binary.
