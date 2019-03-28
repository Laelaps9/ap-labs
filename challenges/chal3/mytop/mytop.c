#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define INFO_LENGTH 30
#define MAX_PROCESSES 1000

void clear();
int readProcesses();
int printProcesses();

struct process {
	char name[INFO_LENGTH],
		 state[INFO_LENGTH],
		 pid[INFO_LENGTH],
		 ppid[INFO_LENGTH],
		 memory[INFO_LENGTH],
		 threads[INFO_LENGTH],
		 openFiles[INFO_LENGTH];
};

struct process processes[MAX_PROCESSES];

int main(){
	// Place your magic here
	readProcesses();
	printProcesses();
	clear();
	return 0;
}

int readProcesses() {
	DIR *d = opendir("/proc/");
	struct dirent *dir;
	FILE *fp;
	char path[30],
		 info[INFO_LENGTH];
	int dataNum,
		procNum = 0,
		c;

	memset(info, 0, INFO_LENGTH);
	strcpy(path, "/proc/");
	while((dir = readdir(d)) != NULL) {

		// Filters out folders starting with a number
		if(dir->d_name[0] >=48 && dir->d_name[0] <=57) {
			dataNum = 0;
			// Add path of directory and status to know which file to enter
			strcat(path, dir->d_name);
			strcat(path, "/status");
			// Read status file
			fp = fopen(path, "r");

			while(dataNum < 6) {
				switch(dataNum) {
					case 0:
						do {
							c = getc(fp);
						} while(c != 'N');

						// Checking to see if what was found was the tag "Name"
						if(getc(fp) == 'a' && getc(fp) == 'm' && getc(fp) == 'e' && getc(fp) == ':') {
							do {
								c = getc(fp);
							} while(c == ' ');

							for(int j = 0; c != '\n'; j++) {
								info[j] = c;
								c = getc(fp);
							}
							strcpy(processes[procNum].name, info);
						}
						else {
							dataNum = 0;
						}
						break;
						case 1:
							do {
								c = getc(fp);
							} while(c != 'P');

							// Checking to see if what was found was the tag "Pid"
							if(getc(fp) == 'i' && getc(fp) == 'd' && getc(fp) == ':') {
								do {
									c = getc(fp);
								} while(c == ' ');

								for(int j = 0; c != '\n'; j++) {
									info[j] = c;
									c = getc(fp);
								}
								strcpy(processes[procNum].pid, info);
							}
							else {
								dataNum = 1;
							}
							break;
				}
				memset(info, 0, INFO_LENGTH);
				dataNum++;
				c = '\0';
			}
			strcpy(path, "/proc/");
			fclose(fp);
			procNum++;
		}
	}
	closedir(d);
	return 0;
}

int printProcesses() {
	int i = 0;
	while(processes[i].name[0] != '\0') {
		printf("Name: %s - Pid: %s\n", processes[i].name, processes[i].pid);
		i++;
	}
	return 0;
}

void clear() {
	printf("\e[1;1H\e[2J");
}
