#include <stdio.h>
#include <stdarg.h>

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

	textcolor(BRIGHT, WHITE, NO_BACKGROUND);

	// printf source code
	va_start (arg, format);
	done = vfprintf (stdout, format, arg);
	va_end (arg);

	// End the color change
	printf("\033[0m");
	return done;
}

int warnf(const char *format, ...) {
	va_list arg;
	int done;

	textcolor(BRIGHT, MAGENTA, NO_BACKGROUND);
	va_start (arg, format);
	done = vfprintf (stdout, format, arg);
	va_end (arg);
	printf("\033[0m");
	return done;
}

int errorf(const char *format, ...) {
	va_list arg;
	int done;

	textcolor(BRIGHT, RED, NO_BACKGROUND);
	va_start (arg, format);
	done = vfprintf (stdout, format, arg);
	va_end (arg);
	printf("\033[0m");
	return done;
}

int panicf(const char *format, ...) {
	va_list arg;
	int done;

	textcolor(REVERSE, MAGENTA, NO_BACKGROUND);
	va_start (arg, format);
	done = vfprintf (stdout, format, arg);
	va_end (arg);
	printf("\033[0m");
	return done;
}

int main() {
	infof("INFO\n");
	warnf("WARNING\n");
	errorf("ERROR\n");
	panicf("PANIC\n");
}
