# Lab 2.4
## logger.c
**logger.c** is a library that contains the functions used for printing each type of log. The user can specify where
to print these logs (stdout or syslog) using the function **initLogger**. Using initLogger("") and initLogger("stdout") will change the logs'
destination to stdout, using initLogger("syslog") will change the logs' destination to syslog. Not using initLogger() will have stdout as default.
* INFO: Info. messages are printed in white, bold color. In syslog this log will be printed in *LOG_INFO* format.  
* WARNING: Warnings are printed in a dim blue color. In syslog this log will be printed in *LOG_WARNING* format.  
* ERROR: Errors are printed in red. In syslog this log will be printed in *LOG_ERR* format.  
* PANIC: Panics are printed in black text color but the background is changed to pink. In syslog this log will be printed in *LOG_CRIT* format.  
**logger.h** is the header file of the logger.c functions
## testLogger.c
**testLogger.c** has a main in which one of each type of log is printed, one for the default destination, one for stdout, one for syslog and done
for stdout but using an empty string in the **initLogger** function.
## Compiling and running
By typing `make` in terminal, you will be creating two output files, **logger.o** and **testLogger.o**. These files are then compiled together,
outputting the binary file **testLogger**. To run this test, type `./testLogger` in terminal. Finally, just type `make clean` to delete all the
output files created with `make`.
