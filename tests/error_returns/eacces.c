/* eacces.c */
/* Tests to see if EACCES properly returned by perf_event_open() */

/* By Vince Weaver vincent.weaver _at_ maine.edu */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include <errno.h>

#include "perf_event.h"
#include "perf_helpers.h"
#include "test_utils.h"

int main(int argc, char **argv) {

	int fd,failures=0;
	int quiet;
	struct perf_event_attr attr;

	char test_string[]="Testing EACCES generation...";

	quiet=test_quiet();

	if (!quiet) {
		printf("This tries to get perf_event_open() to generate\n");
		printf("an EACCES return.\n\n");
	}


	/******************************************/
	/* Check attaching to CPU with any thread */
	/* FIXME: check paranoid value            */
	/*        to avoid false failures         */
	/******************************************/

	memset(&attr,0,sizeof(struct perf_event_attr));
	attr.type=PERF_TYPE_HARDWARE;
	attr.config=PERF_COUNT_HW_INSTRUCTIONS;

	fd=perf_event_open(&attr,
				-1,	/* any thread */
				0,	/* cpu 0 */
				-1,	/* New Group Leader */
				0	/*0*/ );

	if (fd<0) {
		if (errno==EACCES) {
			if (!quiet) {
				printf("Properly triggered EACCES\n");
			}
		}
		else {
			if (!quiet) {
				printf("Unexpectedly got: %s\n",strerror(errno));
			}
			failures++;
		}
	}
	else {
		if (!quiet) {
			printf("Unexpectedly opened properly.\n");
		}
		failures++;
		close(fd);
	}

	/**********************************************/
	/* Check attaching to thread that's not ours  */
	/**********************************************/

	memset(&attr,0,sizeof(struct perf_event_attr));
	attr.type=PERF_TYPE_HARDWARE;
	attr.config=PERF_COUNT_HW_INSTRUCTIONS;

	fd=perf_event_open(&attr,
				1,	/* init */
				-1,	/* any cpu */
				-1,	/* New Group Leader */
				0	/*0*/ );

	if (fd<0) {
		if (errno==EACCES) {
			if (!quiet) {
				printf("Properly triggered EACCES\n");
			}
		}
		else {
			if (!quiet) {
				printf("Unexpectedly got: %s\n",strerror(errno));
			}
			failures++;
		}
	}
	else {
		if (!quiet) {
			printf("Unexpectedly opened properly.\n");
		}
		failures++;
		close(fd);
	}


	/* Done and report */

	if (failures) {
		test_fail(test_string);
	}

	test_pass(test_string);

	return 0;
}
