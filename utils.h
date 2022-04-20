#ifndef _UTILS_H_
#define _UTILS_H_

#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>    
#include <stdint.h>
#include <inttypes.h>
#include <fcntl.h>
#include <pthread.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/eventfd.h>

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec);

int64_t micros();

#endif
