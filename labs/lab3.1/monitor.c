#include <sys/inotify.h>
#include <limits.h>
#include <stdio.h>
#include "logger.h"
#define _XOPEN_SOURCE 500   /* See feature_test_macros(7) */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ftw.h>
#include <stdint.h>

int renamed, inotifyFd;
uint32_t cookie2 = 0, len = 0;
char *name;

static int
walkDirs(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
	if(inotifyFd == -1) {
		errorf("inotify_init");
	}

	if(tflag == FTW_D) {
		printf("Directory %s\n", fpath);
		inotify_add_watch(inotifyFd, fpath, IN_ALL_EVENTS);
	}
    return 0;           /* To tell nftw() to continue */
}

static void             /* Display information from inotify_event structure */
displayInotifyEvent(struct inotify_event *i) {
    if (i->mask & IN_CREATE)  {
		infof("    wd =%2d; ", i->wd);

		if(cookie2 > 0) {
			infof("mask = IN_MOVED_FROM ");

			if (len > 0) {
		        infof("        name = %s\n", name);
			}

			cookie2 = 0;
			len = 0;
			strcpy(name, "");
		}

		infof("mask = IN_CREATE ");
		if (i->len > 0) {
	        infof("        name = %s\n", i->name);
			cookie2 = i->cookie;
		}
	}
	if (i->mask & IN_DELETE)  {
		infof("    wd =%2d; ", i->wd);

	    if (i->cookie > 0)
	        infof("cookie =%4d; ", i->cookie);

		infof("mask = IN_DELETE ");
		if (i->len > 0)
	        infof("        name = %s\n", i->name);
	}
	if (i->mask & IN_DELETE_SELF)  {
		infof("    wd =%2d; ", i->wd);

	    if (i->cookie > 0)
	        infof("cookie =%4d; ", i->cookie);

		infof("mask = IN_DELETE_SELF ");
		if (i->len > 0)
	        infof("        name = %s\n", i->name);
	}

	if (i->mask & IN_MOVED_FROM)  {
		cookie2 = i->cookie;
		len = i->len;
		name = i->name;
	}

	if (i->mask & IN_MOVED_TO)  {
		infof("    wd =%2d; ", i->wd);

	    if (i->cookie == cookie2)
	        infof("se movió cookie =%4d; ", i->cookie);

		infof("mask = IN_MOVED_TO ");
		if (i->len > 0)
	        infof("        name = %s\n", i->name);
	}
}

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int
main(int argc, char *argv[]) {
    int wd, j, flags;
    char buf[BUF_LEN] __attribute__ ((aligned(8)));
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

	inotifyFd = inotify_init();
	flags = 0;

	if (nftw((argc < 2) ? "." : argv[1], walkDirs, 20, flags)
             == -1) {
         perror("nftw");
         exit(EXIT_FAILURE);
     }

    /* For each command-line argument, add a watch for all events */

    for (;;) {                                  /* Read events forever */
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0)
            errorf("read() from inotify fd returned 0!");

        if (numRead == -1)
            errorf("read");

        /* Process all of the events in buffer returned by read() */

        for (p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }
    exit(EXIT_SUCCESS);
}
