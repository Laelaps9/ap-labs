# Lab 2.3  
## cross-ref.c  
**cross-ref.c** receives a file as a parameter and the proceeds to count the words in the file and how many times they repeat.  
## Compiling and running  
To compile, type `make` in terminal, this will create a binary file named **cross-ref**. To run this program simply type `cross-ref [file to check]`.  
## Specifications  
**cross-ref.c** ignores every word with 2 letters or less and avoids other noise words such as *and*, *the*, etc.  
The list of words and the lines they appear in will be stored in a file named **words.txt**.  
To clean the directory (erase binaries and words.txt) simply type `make clean` in terminal.
