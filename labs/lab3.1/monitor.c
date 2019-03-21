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

int inotifyFd, wdDeleted, wdMoved;
uint32_t cookie2 = 0, len = 0;
char name[20], *path;

static int
walkDirs(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
	if(inotifyFd == -1) {
		errorf("inotify_init");
	}

	if(tflag == FTW_D) {
		infof("Watching %s\n", fpath);
		inotify_add_watch(inotifyFd, fpath, IN_CREATE | IN_DELETE_SELF | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM | IN_ISDIR | IN_MOVE_SELF);
	}
    return 0;           /* To tell nftw() to continue */
}

void cleanWatch() {

}

static void             /* Display information from inotify_event structure */
displayInotifyEvent(struct inotify_event *i) {
    if (i->mask & IN_CREATE)  {
		infof("    wd =%2d; ", i->wd);

		infof("mask = IN_CREATE ");
		if (i->len > 0) {
	        infof("        name = %s\n", i->name);
			cookie2 = i->cookie;
		}

		// Check if what was created was a directory
		if(i->mask & IN_ISDIR) {
			infof("\n\t%s was added to the monitored subdirectories\n", i->name);
			infof("\n");
			// Get the watchs again
			if (nftw(path, walkDirs, 20, 0) == -1) {
		         perror("nftw");
		         exit(EXIT_FAILURE);
		     }
		}

		if(len > 0) {
			cookie2 = 0;
			len = 0;
			memset(name, 0, 20);
		}
	}

	if (i->mask & IN_DELETE)  {
		infof("    wd =%2d; ", i->wd);

	    if (i->cookie > 0)
	        infof("cookie =%4d; ", i->cookie);

		infof("mask = IN_DELETE ");
		if (i->len > 0)
	        infof("        name = %s\n", i->name);

		if(len > 0) {
			cookie2 = 0;
			len = 0;
			memset(name, 0, 20);
		}

		if(wdDeleted > 0) {
			inotify_rm_watch(inotifyFd, wdDeleted);
			wdDeleted = 0;
		}
	}

	if (i->mask & IN_DELETE_SELF)  {
		wdDeleted = i->wd;
	}

	if ((i->mask & IN_MOVE_SELF) && (i->mask & IN_ISDIR))  {
		inotify_rm_watch(inotifyFd, i->wd);
	}

	if (i->mask & IN_MOVED_FROM)  {
		cookie2 = i->cookie;
		len = i->len;
		strcpy(name, i->name);
		if(i->mask & IN_ISDIR) {
			if (nftw(path, walkDirs, 20, 0) == -1) {
				perror("nftw");
				exit(EXIT_FAILURE);
			}
		}
	}

	if (i->mask & IN_MOVED_TO )  {

	    if (i->cookie == cookie2 && (strcmp(name, i->name) != 0)) {
			infof("    wd =%2d; ", i->wd);
			infof("mask = RENAME ");
			if (i->len > 0) {
			    infof("        FROM %s TO %s\n", name, i->name);
			}
		}
		else {
			 if (i->mask & IN_ISDIR) {
				 if(strcmp(name, i->name) != 0) {
					 if (nftw(path, walkDirs, 20, 0) == -1) {
		 				perror("nftw");
		 				exit(EXIT_FAILURE);
		 			 }
				 }
	 		}
		}
		if(len > 0) {
			cookie2 = 0;
			len = 0;
			memset(name, 0, 20);

		}
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

	// If no path is given, program will check the current directory by default
	if(argc < 2) {
		path = ".";
	}
	else {
		path = argv[1];
	}

	infof("\n");

	// Find all subdirectories in directory to check and add a watch to them
	if (nftw(path, walkDirs, 20, flags) == -1) {
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
