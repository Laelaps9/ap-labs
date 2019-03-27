#include <stdio.h>
#include <dirent.h>
#include <string.h>

void clear();

int main(){
	// Place your magic here
	DIR *d = opendir("/proc/");
	struct dirent *dir;
	FILE *fp;
	char path[30],
		 c;
	int p = 0;

	strcpy(path, "/proc/");
	while((dir = readdir(d)) != NULL) {

		// Filters out folders starting with a number
		if(dir->d_name[0] >=48 && dir->d_name[0] <=57) {

			// Add path of directory and status to know which file to enter
			strcat(path, dir->d_name);
			strcat(path, "/status");
			printf("%s\n", path);
			// Read status file
			fp = fopen(path, "r");
			// Find PID
			while((c = getc(fp)) != 'P') {
			}
			// Print PID and PPID
			while(p < 2) {
				while((c = getc(fp)) != '\n') {
					printf("%c", c);
				}
				printf("\n");
				p++;
			}
			printf("\n");
			p = 0;
			strcpy(path, "/proc/");
		}

	}

	clear();
	return 0;
}

void clear() {
	printf("\e[1;1H\e[2J");
}
