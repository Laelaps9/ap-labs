#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *fp;
    char *filename = argv[1];
    int c;

    if(!(fp = fopen(filename, "r"))) {
        printf("File not found\n");
        return 1;
    }
    // Get the first char in a file to check before printing
    c = getc(fp);
    // Loops until the end of file is found
    while(!(feof(fp)) && c != EOF) {
        printf("%c", c);
        c = getc(fp);
    }

    fclose(fp);
    return 0;
}
