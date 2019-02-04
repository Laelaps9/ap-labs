#include <stdio.h>

int stringlen(char *str);
char* stradd(char *origin, char *addition);
int strfind(char *origin, char *substr);

int main(int argc, char **argv) {
    char *newString = stradd(argv[1], argv[2]);
    printf("Initial length      : %d\n", stringlen(argv[1]));
    printf("New String          : %s\n", newString);
    if(strfind(newString, argv[3])) {
        printf("SubString was found : yes\n");
    }
    else {
        printf("SubString was found : no\n");
    }
}
