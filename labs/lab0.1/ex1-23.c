#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    int c,
        checking = 1,
        lineSkip = 0;
    fpos_t pos,
           pos2;
    char *filename = (char *)malloc(sizeof(char) * 20);

    printf("Type the name of the file: ");
    scanf("%s", filename);
    printf("%s", filename);
    fp = fopen(filename, "r+");
    if(fp) {
        while(!feof(fp)) {
            fgetpos(fp, &pos);
            c = getc(fp);
            if(c == '/') {
                c = getc(fp);
                switch(c) {
                    case '/':
                        fsetpos(fp, &pos);
                        while(ungetc(getc(fp), fp) != '\n' && !feof(fp)) {
                            fputc(' ', fp);
                        }
                        fsetpos(fp, &pos);
                        fputc(' ', fp);
                        break;
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
                }
            }
            else if(c == '"') {
                do {
                    c = getc(fp);
                } while(c != '"');
            }
        }
    fclose(fp);
    }
}
