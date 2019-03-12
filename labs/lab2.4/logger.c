#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include "logger.h"

#define RESET			0
#define BRIGHT 			1
#define DIM				2
#define UNDERLINE 		3
#define BLINK			4
#define REVERSE			7
#define HIDDEN			8

#define BLACK 			0
#define RED				1
#define GREEN			2
#define YELLOW			3
#define BLUE			4
#define MAGENTA			5
#define CYAN			6
#define	WHITE			7
#define NO_BACKGROUND  -1

// Variable that will represent if the message is to be printed in stdout or syslog
int msgLoc = 1; // 1 is for stdout, 0 is for syslog. stdout is the default

int initLogger(char *logType) {
	// If logType is "stdout" or an empty string, messages will be printed to stdout
	if((strcmp(logType, "stdout") == 0) || (logType[0] == '\0')) {
		msgLoc = 1;
		printf("Initializing Logger on: stdout\n");
	}
	else if(strcmp(logType, "syslog") == 0) {
		msgLoc = 0;
		printf("Initializing Logger on: %s\n", logType);
	}
	else {
		errorf("Not a valid log type\n");
		return -1;
	}
	return 0;
}

void textcolor(int attr, int fg, int bg) {
	char command[13];

	// bg = -1 means that no background color is wanted
	if(bg == -1) {
		sprintf(command, "%c[%d;%dm", 0x1B, attr, fg + 30);
		printf("%s", command);
	}
	else {
		sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
		printf("%s", command);
	}
}

int infof(const char *format, ...) {
	va_list arg;
	int done;

	va_start(arg, format);
	// Printing message in stdout
	if(msgLoc) {
		textcolor(BRIGHT, WHITE, NO_BACKGROUND);
		done = vfprintf(stdout, format, arg);

		// End the color change
		printf("\033[0m");
	}
	// Printing message in syslog
	else {
		openlog("logger", LOG_PID, LOG_USER);
		vsyslog(LOG_INFO, format, arg);
		closelog();
		done = 0;
	}
	va_end(arg);
	return done;
}

int warnf(const char *format, ...) {
	va_list arg;
	int done;

	va_start (arg, format);
	if(msgLoc) {
		textcolor(BRIGHT, MAGENTA, NO_BACKGROUND);
		done = vfprintf (stdout, format, arg);
		printf("\033[0m");
	}
	else {
		openlog("logger", LOG_PID, LOG_USER);
		vsyslog(LOG_WARNING, format, arg);
		closelog();
		done = 0;
	}
	va_end (arg);
	return done;
}

int errorf(const char *format, ...) {
	va_list arg;
	int done;

	va_start (arg, format);
	if(msgLoc) {
		textcolor(BRIGHT, RED, NO_BACKGROUND);
		done = vfprintf (stdout, format, arg);
		printf("\033[0m");
	}
	else {
		openlog("logger", LOG_PID, LOG_USER);
		vsyslog(LOG_ERR, format, arg);
		closelog();
		done = 0;
	}
	va_end (arg);
	return done;
}

int panicf(const char *format, ...) {
	va_list arg;
	int done;

	va_start (arg, format);
	if(msgLoc) {
		textcolor(REVERSE, MAGENTA, NO_BACKGROUND);
		done = vfprintf (stdout, format, arg);
		printf("\033[0m");
	}
	else {
		openlog("logger", LOG_PID, LOG_USER);
		vsyslog(LOG_CRIT, format, arg);
		closelog();
		done = 0;
	}
	va_end (arg);
	return done;
}
