#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char **argv) {
    DIR *d = opendir("/dev/pts/");
    FILE *fp;
    char ttys[20][3],
         msg[51],
         path[12];
    int count = 0;

    // This struct will contain the name of every element in the selected directory
    struct dirent *dir;
    strcpy(path, "/dev/pts/");
    // Make a string with the message to be displayed
    for(int i = 1; i < argc; i++) {
        strcat(msg, argv[i]);
        if(i < argc - 1) {
            strcat(msg, " ");
        }
    }
    strcat(msg, "\n");

    // Read the elements in the directory and save them to the ttys array.
    while((dir = readdir(d)) != NULL) {
        // Ignores them if they are named ".", "..", or "ptmx"
        if(dir->d_name[0] != '.' && strcmp(dir->d_name, "ptmx")) {
            strcpy(ttys[count++], dir->d_name);
        }
    }
    // Loops until all the non-empty strings in the array are checked
    for(int i = 0; i < (sizeof(ttys) / sizeof(ttys[0])) && ttys[i][0] != '\0'; i++) {
        // Append the pts id
        strcat(path, ttys[i]);
        fp = fopen(path, "w");
        for(int j = 0; j < strlen(msg); j++) {
            putc(msg[j], fp);
        }
        fclose(fp);
        // Reset path
        strcpy(path, "/dev/pts/");
    }
    closedir(d);
    return 0;
}
