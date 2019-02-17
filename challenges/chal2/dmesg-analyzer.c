#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

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
    char *entries[LOG_INFO];
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
   }

    // Adds the new entry and increases the nEntries value
    char *copy = (char *)malloc(sizeof(char) * LOG_INFO);
    strcpy(copy, entry);
    log_dict[index].entries[log_dict[index].nEntries++] = copy;
}

void writeReport(int reportfd) {
    for(int i = 0; *log_dict[i].name != '\0'; i++) {
        write(reportfd, strcat(log_dict[i].name, "\n"), strlen(log_dict[i].name) + 1);
        for(int j = 0; j < log_dict[i].nEntries; j++) {
            if(strcmp(log_dict[i].entries[j], "") != 0) {
                write(reportfd, "  ", 2);
                write(reportfd, strcat(log_dict[i].entries[j], "\n"), strlen(log_dict[i].entries[j]) + 1);
            }
        }
    }
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int fd = open(logFile, O_RDONLY),
        fileSize = lseek(fd, 0, SEEK_END),
        log_start,
        type_start,
        type_end,
        type_found = 0,
        new_type = 0,
        reportfd;
    char data[fileSize + 1],
         type_name[TYPE_NAME],
         log_info[LOG_INFO];

    lseek(fd, 0, SEEK_SET);
    read(fd, data, fileSize + 1);
    close(fd);

    // Creates the General log type
    strcpy(log_dict[0].name, "General:");

    for(int i = 0; i < fileSize; i++) {

        if(data[i] == '[' && data[i + 1] == ' ') {
            log_start = i;
        }
        while(data[i] != ']') {
            i++;
        }
        type_start = i + 2;
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
        switch (type_found) {
            case 0:
                for(int j = log_start; data[j] != '\n' && j < fileSize; j++) {
                    log_info[j - log_start] = data[j];
                }
                addEntry("General:", log_info);
                memset(log_info, 0, LOG_INFO);
                break;
            case 1:
                for(int j = type_start; j < type_end + 1; j++) {
                    type_name[j - type_start] = data[j];
                    data[j] = '\\';
                }
                for(int k = log_start; data[k] != '\n'; k++) {
                    if(data[k] == '\\') {
                        log_start++;
                    }
                    else {
                        log_info[k - log_start] = data[k];
                    }
                }
                addEntry(type_name, log_info);
                memset(type_name, 0, TYPE_NAME);
                memset(log_info, 0, LOG_INFO);
                break;
            case 2:
                for(int j = type_start; j < type_end + 1; j++) {
                    type_name[j - type_start] = data[j];
                }
                log_start = type_end + 2;
                while(!new_type) {
                    i = log_start;
                    for(int k = log_start; data[k] != '\n'; k++, log_start++) {
                        if(data[k] == ':' && (data[k + 1] == ' ' || data[k + 1] == '\n')) {
                            new_type = 1;
                            memset(log_info, 0, LOG_INFO);
                            strcpy(log_info, "");
                            break;
                        }
                        log_info[k - i] = data[k];
                    }
                    log_start++;
                    addEntry(type_name, log_info);
                    memset(log_info, 0, LOG_INFO);
                }
                log_start--;
                memset(type_name, 0, TYPE_NAME);
                memset(log_info, 0, LOG_INFO);
                i -= log_start - i;
                break;
        }
        while(data[i] != '\n' && i < fileSize) {
            i++;
        }
        type_found = 0;
        new_type = 0;
    }
    reportfd = open(report, O_RDWR | O_CREAT, 0666);
    writeReport(reportfd);
    close(reportfd);
    printf("Report is generated at: [%s]\n", report);
}
