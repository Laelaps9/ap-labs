#include <stdlib.h>

int stringlen(char *str) {
    int len = 0;

    // Counts all the elements until the null char is found
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

char* stradd(char *origin, char *addition) {
    int newLen = stringlen(origin) + stringlen(addition),
        i;
    // Allocates memory for a new string with the size of both strings combined
    char *newString = (char *)malloc(sizeof(char) * newLen);

    // Copies origin string to the new string
    for(i = 0; origin[i] != '\0'; i++) {
        newString[i] = origin[i];
    }
    // Copies the string to be added to the new string
    for(int j = 0; addition[j] != '\0'; j++, i++) {
        newString[i] = addition[j];
    }
    return newString;
}

int strfind(char *origin, char *substr) {
    int len = stringlen(origin),
        len2 = stringlen(substr),
        foundStr;

    for(int i = 0; i < len; i++) {
        if(origin[i] == substr[0]) {
            foundStr = 1;
            for(int j = 0; j < len2; j++) {

                // Checks if string ends before substring
                if(origin[i + j] == '\0') {
                    return 0;
                }

                // If respective letters are different, starts again
                if(substr[j] != origin[i + j]) {
                    foundStr = 0;
                    break;
                }
            }
            // foundStr will be 1 if all letters were the same, meaning the substring was found
            if(foundStr) {
                return 1;
            }
        }
    }
    return 0;
}
