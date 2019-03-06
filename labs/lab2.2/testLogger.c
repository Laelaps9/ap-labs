#include "logger.h"

int main() {
	infof("This is an info message %d\n", 1);
	warnf("This is a warning %d\n", 2);
	errorf("This is an error %d\n", 3);
	panicf("This is a panic %d\n", 4);
	return 0;
}
