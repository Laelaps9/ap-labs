#include <stdio.h>
#include "logger.h"

int main(){

    // default logging
    infof("INFO Message default %d\n", 1);
    warnf("WARN Message default %d\n", 2);
    errorf("ERROR Message default %d\n", 2);
	panicf("PANIC Message default %d\n", 2);

    // stdout logging
    initLogger("stdout");
    infof("INFO Message stdout %d\n", 1);
    warnf("WARN Message stdout %d\n", 2);
    errorf("ERROR Message stdout %d\n", 2);
	panicf("PANIC Message stdout %d\n", 2);

    // syslog logging
    initLogger("syslog");
    infof("INFO Message syslog %d\n", 1);
    warnf("WARN Message syslog %d\n", 2);
    errorf("ERROR Message syslog %d\n", 2);
	panicf("PANIC Message syslog %d\n", 2);

	// empty string logging
	initLogger("");
    infof("INFO Message "" %d\n", 1);
    warnf("WARN Message "" %d\n", 2);
    errorf("ERROR Message "" %d\n", 2);
	panicf("PANIC Message "" %d\n", 2);

    return 0;
}
