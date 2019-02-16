#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#define REPORT_FILE "report.txt"
#define DICTIONARY_SIZE 10000
#define MAX_ENTRIES 32000
#define TYPE_NAME 100
#define LOG_INFO 300

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./dmesg-analizer.o logfile.txt\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

struct log_type {
    char name[TYPE_NAME];
    char **entries;
    int nEntries;
};

struct log_type log_dict[DICTIONARY_SIZE];

// Returns the position of the log type inside the dictionary
// Returns -1 if the category isn't found
int hasLogType(char *type) {
    for(int i = 0; *log_dict[i].name != '\0'; i++) {
        if(strcmp(log_dict[i].name, type) == 0) {
            return i;
        }
    }
    return -1;
}

void addEntry(char *type, char entry[]) {
    int index = hasLogType(type);
    // If the log type does not exit in the dictionar, it is assigned
    if(index == -1) {
        index = 0;
        while(*log_dict[index].name != '\0') {
            index++;
        }
        strcpy(log_dict[index].name, type);
        log_dict[index].nEntries = 0;
        for(int i = 0; i < MAX_ENTRIES; i++) {
            log_dict[index].entries[i] = "NULL";
        }
    }
    // Adds the new entry and increases the nEntries value
    log_dict[index].entries[log_dict[index].nEntries++] = entry;
}

void printDict() {
    for(int i = 0; *log_dict[i].name != '\0'; i++) {
        printf("%s\n", log_dict[i].name);
        for(int j = 0; j < log_dict[i].nEntries; j++) {
            printf("\t%s\n", log_dict[i].entries[j]);
        }
    }
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int filedesc = open(logFile, O_RDONLY),
        fileSize = lseek(filedesc, 0, SEEK_END),
        log_start,
        type_start,
        type_end,
        type_found = 0;
    char data[fileSize + 1],
         type_name[TYPE_NAME],
         *log_info = (char *)calloc(LOG_INFO, sizeof(char));
//         *entry = (char *)calloc(LOG_INFO, sizeof(char));

    lseek(filedesc, 0, SEEK_SET);
    read(filedesc, data, fileSize + 1);
    //printf("%s\n", data);
/*    addEntry("hola", "Entry 0");
    addEntry("hola2", "Entry 1");
    addEntry("hola2", "Entry 2");
    addEntry("hola3", "Entry 3");
    printDict();
*/

    // Creates the General log type
    strcpy(log_dict[0].name, "General:");

    for(int i = 0; i < fileSize + 1; i++) {

        if(data[i] == '[' && data[i + 1] == ' ') {
            log_start = i;
        }
        while(data[i] != ']') {
            i++;
        }
        type_start = i;
        while((data[i] != '\n' && i < fileSize) && type_found == 0) {
            if(data[i] == ':' && data[i + 1] == ' ') {
                type_found = 1;
            }
            else if(data[i] == ':' && data[i + 1] == '\n') {
                type_found = 2;
            }
            else {
                i++;
            }
        }
        type_end = i;
        printf("basura\n");
        switch (type_found) {
            case 0:
                for(int j = log_start; data[j] != '\n' && j < fileSize; j++) {
                    printf("%c", data[j]);
                    log_info[j - log_start] = data[j];
    //                printf("%c", log_info[j - log_start]);
                }
    //            printf("\n");
        //        strcpy(log_info[log_counter], entry);
                addEntry("General:", log_info);
    //            memset(entry, 0, LOG_INFO);
                break;
        }
        while(data[i] != '\n' && i < fileSize) {
            i++;
        }
        type_found = 0;
    }
//    printDict();
    printf("Report is generated at: [%s]\n", report);
}
