#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    int c,
        checking = 1;
    fpos_t pos,
           pos2;
    // Allocates filename so that it can receive a name
    char *filename = (char *)malloc(sizeof(char) * 20);

    printf("Type the name of the file: ");
    scanf("%s", filename);

    // Checks if a file with the name entered exists
    // Exits if it does not
    if(!(fp = fopen(filename, "r+"))) {
        printf("%s\n", "File not found");
        return 1;
    }
    while(!feof(fp)) {
        // Saves the position of each character read to know where a comment might start
        fgetpos(fp, &pos);
        c = getc(fp);
        if(c == '/') {
            c = getc(fp);
            switch(c) {
                // Deletes single-line comments
                case '/':
                    fsetpos(fp, &pos);
                    while(ungetc(getc(fp), fp) != '\n' && !feof(fp)) {
                        fputc(' ', fp);
                    }
                    fsetpos(fp, &pos);
                    fputc(' ', fp);
                    break;
                // Deletes multi-line comments
                case '*':
                    do {
                        while(ungetc(getc(fp), fp) != '*') {
                            fputc(' ', fp);
                        }
                        fgetpos(fp, &pos2);
                        getc(fp);
                        if(ungetc(getc(fp), fp) == '/') {
                            fsetpos(fp, &pos);
                            fputc(' ', fp);
                            fputc(' ', fp);
                            checking = 0;
                        }
                        else {
                            fsetpos(fp, &pos2);
                            fputc(' ', fp);
                            checking = 1;
                        }
                    } while(checking);
                    fsetpos(fp, &pos2);
                    fputc(' ', fp);
                    fputc(' ', fp);
                    break;
                // Program keeps running normally if no comment was detected
            }
        }
        // Checks the contents of a string
        else if(c == '"') {
            do {
                c = getc(fp);
                // Ignores character constants in the string
                if(c == '\\') {
                    getc(fp);
                    c = getc(fp);
                }
            } while(c != '"');
        }
    }
    fclose(fp);
    printf("%s\n", "Comments deleted");
    return 0;
}
