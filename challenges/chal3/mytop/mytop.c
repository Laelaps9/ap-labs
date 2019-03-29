#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define INFO_LENGTH 40
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
		 threads[INFO_LENGTH];
	int openFiles;
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
		 fdpath[30],
		 info[INFO_LENGTH],
		 c;
	int dataNum,
		procNum = 0,
		opFiles;

	memset(info, 0, INFO_LENGTH);
	strcpy(path, "/proc/");
	strcpy(fdpath, "/proc/");
	while((dir = readdir(d)) != NULL) {

		// Filters out folders starting with a number
		if(dir->d_name[0] >= 48 && dir->d_name[0] <= 57) {
			dataNum = 0;
			opFiles = 0;
			// Add status path
			strcat(path, dir->d_name);
			strcat(path, "/status");

			// Add fd path
			strcat(fdpath, dir->d_name);
			strcat(fdpath, "/fd");
			// Read status file
			fp = fopen(path, "r");

			while(dataNum < 6) {
				switch(dataNum) {
					case 0:
						do {
							c = getc(fp);
						} while(c != 'N' && c != EOF);

						if(c == EOF) {
							fseek(fp, 0L, SEEK_SET);
							strcpy(processes[procNum].name, "");
							break;
						}

						// Checking to see if what was found was the tag "Name"
						if(getc(fp) == 'a' && getc(fp) == 'm' && getc(fp) == 'e' && getc(fp) == ':') {
							do {
								c = getc(fp);
							} while(c == ' ' || c == '\t');

							for(int j = 0; c != '\n'; j++) {
								info[j] = c;
								c = getc(fp);
							}
							strcpy(processes[procNum].name, info);

							// Go back to the beginning of the file
							fseek(fp, 0L, SEEK_SET);
						}
						else {
							dataNum = -1;
						}
						break;
						case 1:
							do {
								c = getc(fp);
							} while(c != 'S' && c != EOF);

							if(c == EOF) {
								fseek(fp, 0L, SEEK_SET);
								break;
							}

							// Checking to see if what was found was the tag "State"
							if(getc(fp) == 't' && getc(fp) == 'a' && getc(fp) == 't' && getc(fp) == 'e' && getc(fp) == ':') {
								do {
									c = getc(fp);
								} while(c != '(');

								c = getc(fp);

								for(int j = 0; c != ')'; j++) {
									info[j] = c;
									c = getc(fp);
								}
								strcpy(processes[procNum].state, info);
								fseek(fp, 0L, SEEK_SET);
							}
							else {
								dataNum = 0;
							}
							break;
							case 2:
								do {
									c = getc(fp);
								} while(c != 'P' && c != EOF);

								if(c == EOF) {
									fseek(fp, 0L, SEEK_SET);
			//						strcpy(processes[procNum].pid, "");
									break;
								}

								// Checking to see if what was found was the tag "Pid"
								if(getc(fp) == 'i' && getc(fp) == 'd' && getc(fp) == ':') {
									do {
										c = getc(fp);
									} while(c == ' ' || c == '\t');

									for(int j = 0; c != '\n'; j++) {
										info[j] = c;
										c = getc(fp);
									}
									strcpy(processes[procNum].pid, info);

									fseek(fp, 0L, SEEK_SET);

								}
								else {
									dataNum = 1;
								}
								break;
							case 3:
								do {
									c = getc(fp);
								} while(c != 'P' && c != EOF);

								if(c == EOF) {
									fseek(fp, 0L, SEEK_SET);
									break;
								}

								// Checking to see if what was found was the tag "PPid"
								if(getc(fp) == 'P' && getc(fp) == 'i' && getc(fp) == 'd' && getc(fp) == ':') {
									do {
										c = getc(fp);
									} while(c == ' ' || c == '\t');

									for(int j = 0; c != '\n'; j++) {
										info[j] = c;
										c = getc(fp);
									}
									strcpy(processes[procNum].ppid, info);
									fseek(fp, 0L, SEEK_SET);
								}
								else {
									dataNum = 2;
								}
								break;
								case 4:
									do {
										c = getc(fp);
									} while(c != 'V' && c != EOF);
									if(c == EOF) {
										fseek(fp, 0L, SEEK_SET);
										break;
									}

									// Checking to see if what was found was the tag "VmRSS"
									if(getc(fp) == 'm' && getc(fp) == 'R' && getc(fp) == 'S' && getc(fp) == 'S' && getc(fp) == ':') {
										do {
											c = getc(fp);
										} while(c == ' ' || c == '\t');

										for(int j = 0; c != ' '; j++) {
											info[j] = c;
											c = getc(fp);
										}
										strcpy(processes[procNum].memory, info);

										fseek(fp, 0L, SEEK_SET);
									}
									else {
										dataNum = 3;
									}
									break;
									case 5:
										do {
											c = getc(fp);
										} while(c != 'T' && c != EOF);

										if(c == EOF) {
											fseek(fp, 0L, SEEK_SET);
											break;
										}

										// Checking to see if what was found was the tag "Threads"
										if(getc(fp) == 'h' && getc(fp) == 'r' && getc(fp) == 'e' && getc(fp) == 'a' && getc(fp) == 'd' && getc(fp) == 's' && getc(fp) == ':') {

											do {
												c = getc(fp);
											} while(c == ' ' || c == '\t');

											if(c == '\n') {
												break;
											}

											for(int j = 0; c != '\n'; j++) {
												info[j] = c;
												c = getc(fp);
											}
											strcpy(processes[procNum].threads, info);
										}
										else {
											dataNum = 4;
										}
										break;
				}
				memset(info, 0, INFO_LENGTH);
				dataNum++;
				c = '\0';
			}

			fclose(fp);

			// Check fd directory for files open
			DIR *fdd = opendir(fdpath);
			struct dirent *fd_dir;

			// Count files
			while((fd_dir = readdir(fdd)) != NULL) {
				opFiles++;
			}

			closedir(fdd);

			// Save # of files excluding "." and ".."
			processes[procNum].openFiles = opFiles - 2;

			strcpy(fdpath, "/proc/");
			strcpy(path, "/proc/");
			procNum++;
		}
	}
	closedir(d);
	return 0;
}

int printProcesses() {
	int i = 0;
	double mem;

	printf("|%-6s | %-5s | %-40s | %-10s | %-10s | %-4s | %-10s|\n", " PID", "Parent", "Name", "State", "Memory", "# Threads", "Open Files");
	printf("|-------|--------|------------------------------------------|------------|------------|-----------|-----------|\n");

	while(processes[i].name[0] != '\0') {

		// convert memory from K to M
		mem = atof(processes[i].memory) / 1000;
		printf("| %-5s | %-6s | %-40s | %-10s | %-9.1fM | %-9s | %-10d|\n", processes[i].pid, processes[i].ppid, processes[i].name, processes[i].state, mem, processes[i].threads, processes[i].openFiles);
		i++;
	}
	return 0;
}

void clear() {
	printf("\e[1;1H\e[2J");
}
