Lab 0.2
=======
## strlib.c
Contains 3 functions:  
> - stringlen()  
> - stradd()  
> - strfind()  
**stringlen()** counts the elements of a string until the null char is found, then returns that number of chars that were counted. The function was named like that to avoid getting warnings about **strlen()** already existing as a built-in function.  
**stradd()** creates a string with size equivalent to the sum of boths strings' length then copies them into it.  
**strfind()** finds the substring specified in the string returned by **stradd()**. Returns *yes* if the substring is found and *no* if it isn't.  
## str-main.c
Tests the functions in strlib.c and formats the answers for easier understanding of the outputs.  
## Compiling and running
To compile the programs simply type ¨make¨ in terminal. The make file will automatically create and link the output files.  
To execute type ¨./str-final.o *original string* *string to add* *substring to check for*¨. **str-final.o** is the output file created after linking the other output files, the strings after are the parameters it receives.  
Finally, if you would like to clean your directory and erase all the output files, type ¨make clean¨.
