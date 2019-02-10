Lab 1.2
=======
## broadcaster.c
**broadcaster.c** goes into the /dev/pts/ directory and stores all the ids in an array. Each of the files is then opened and the message is displayed in every tty. 
## Compiling and running
To compile type `make` in terminal. The output file **broadcaster** will be created. To run the program just type `broadcaster [message to broadcast]`.
## Restrictions
This program only supports up to 20 open terminals and messages with less than 50 characters. If you would like to change these you can modify the declaration of **ttys** and **msg** **in broadcaster.c**. For example, if you would like to be able to broadcast a message with 100 characters to 50 terminals, you could change these variables to **ttys[50][3]** and **msg[101]** *101 because we are taking into account the newline character at the end of the message*. 
