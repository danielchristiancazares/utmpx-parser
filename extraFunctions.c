#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "mywho.h"

static int compareLong(long a, long b)
{
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    return 0;
}

int
idleTimeSortAscending(const void *p1, const void *p2)
{
    const struct utmpInfo *left = (const struct utmpInfo *)p1;
    const struct utmpInfo *right = (const struct utmpInfo *)p2;

    return compareLong((long)left->idle, (long)right->idle);
}

int
idleTimeSortDescending(const void *p1, const void *p2)
{
    return idleTimeSortAscending(p2, p1);
}

int
nameSortAscending(const void *p1, const void *p2)
{
    const struct utmpInfo *left = (const struct utmpInfo *)p1;
    const struct utmpInfo *right = (const struct utmpInfo *)p2;

    return strcmp(left->user, right->user);
}

int
nameSortDescending(const void *p1, const void *p2)
{
    return nameSortAscending(p2, p1);
}

int
timeSortAscending(const void *p1, const void *p2)
{
    const struct utmpInfo *left = (const struct utmpInfo *)p1;
    const struct utmpInfo *right = (const struct utmpInfo *)p2;

    return compareLong((long)left->time, (long)right->time);
}

int
timeSortDescending(const void *p1, const void *p2)
{
    return timeSortAscending(p2, p1);
}

long
strToLong(char *str, int base)
{
    char *endPtr;
    long value;

    if (str == NULL || *str == '\0') {
        errno = EINVAL;
        return -1;
    }

    errno = 0;
    value = strtol(str, &endPtr, base);
    if (errno == ERANGE) {
        return -1;
    }

    while (*endPtr != '\0') {
        if (!isspace((unsigned char)*endPtr)) {
            errno = EINVAL;
            return -1;
        }
        ++endPtr;
    }

    return value;
}

void
doUptime(struct utmpInfo *utmpTable, int entries)
{
    int i;
    int users = 0;
    time_t now = pa4time(NULL);
    time_t bootTime = 0;
    long uptimeSeconds;
    long days;
    long hours;
    long minutes;

    if (utmpTable == NULL || entries <= 0) {
        return;
    }

    for (i = 0; i < entries; ++i) {
        if (utmpTable[i].type == BOOT_TIME) {
            bootTime = utmpTable[i].time;
        }
        if (utmpTable[i].type == USER_PROCESS) {
            ++users;
        }
    }

    if (bootTime == 0) {
        return;
    }

    uptimeSeconds = (long)difftime(now, bootTime);
    if (uptimeSeconds < 0) {
        uptimeSeconds = 0;
    }

    days = uptimeSeconds / 86400;
    hours = (uptimeSeconds % 86400) / 3600;
    minutes = (uptimeSeconds % 3600) / 60;

    (void)fprintf(stdout, "up %ld day(s), %02ld:%02ld, %d user(s)\n", days, hours, minutes, users);
}

void
currentTerminalInfo(struct utmpInfo *utmpTable, int entries)
{
    char *ttyPath;
    char *ttyLine;
    int i;

    if (utmpTable == NULL || entries <= 0) {
        return;
    }

    ttyPath = ttyname(STDIN_FILENO);
    if (ttyPath == NULL) {
        return;
    }

    ttyLine = strrchr(ttyPath, '/');
    ttyLine = (ttyLine == NULL) ? ttyPath : ttyLine + 1;

    for (i = 0; i < entries; ++i) {
        if (utmpTable[i].type == USER_PROCESS && strcmp(utmpTable[i].line, ttyLine) == 0) {
            (void)fprintf(stdout, "%s %s %ld\n", utmpTable[i].user, utmpTable[i].line, (long)utmpTable[i].pid);
            return;
        }
    }
}
