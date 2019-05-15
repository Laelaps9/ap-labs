# Lab 4.2

## multiplier
**multiplier.c** takes two files **matA.dat** and **matB.dat** and then multiplies them following the method to multiply matrices. It creates
2000 * 2000 threads to multiply each row from matA with each column from matB.

## mat.dat
The matrix files must be in the same directory as **multiplier.c**. These files contain the matrices to be multiplied.

## Compiling and Running
To compile the program simply type `make` and this will create the binary **multiplier**.
To run the program type `./multiplier -n <# of buffers>`
