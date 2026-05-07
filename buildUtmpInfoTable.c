#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <utmpx.h>

#include "mywho.h"

int
buildUtmpInfoTable(const char *utmpFilename, struct utmpInfo **utmpInfoPtr)
{
    FILE *utmpFilePtr;
    struct utmpx utmpxBuf;
    int tableSize = 64;
    int numElements = 0;

    if (utmpFilename == NULL || utmpInfoPtr == NULL) {
        errno = EINVAL;
        perror("buildUtmpInfoTable");
        return -1;
    }

    utmpFilePtr = fopen(utmpFilename, "rb");
    if (utmpFilePtr == NULL) {
        perror(utmpFilename);
        return -1;
    }

    *utmpInfoPtr = (struct utmpInfo *)calloc((size_t)tableSize, sizeof(struct utmpInfo));
    if (*utmpInfoPtr == NULL) {
        perror("calloc");
        fclose(utmpFilePtr);
        return -1;
    }

    while (fread(&utmpxBuf, sizeof(utmpxBuf), 1, utmpFilePtr) == 1) {
        struct utmpInfo *entry;
        struct stat statBuffer;
        time_t currentTime;
        char path[BUFSIZ];

        if (numElements == tableSize) {
            struct utmpInfo *resized;
            tableSize *= 2;
            resized = (struct utmpInfo *)realloc(*utmpInfoPtr, (size_t)tableSize * sizeof(struct utmpInfo));
            if (resized == NULL) {
                perror("realloc");
                freeUtmpInfoTable(*utmpInfoPtr, numElements);
                fclose(utmpFilePtr);
                *utmpInfoPtr = NULL;
                return -1;
            }
            *utmpInfoPtr = resized;
        }

        entry = &((*utmpInfoPtr)[numElements]);

        (void)snprintf(entry->user, sizeof(entry->user), "%.*s", (int)sizeof(entry->user) - 1, utmpxBuf.ut_user);
        (void)snprintf(entry->line, sizeof(entry->line), "%.*s", (int)sizeof(entry->line) - 1, utmpxBuf.ut_line);
        entry->type = utmpxBuf.ut_type;
        entry->pid = utmpxBuf.ut_pid;
        entry->time = utmpxBuf.ut_tv.tv_sec;

        (void)snprintf(path, sizeof(path), "/dev/%.*s", (int)sizeof(utmpxBuf.ut_line), utmpxBuf.ut_line);
        if (stat(path, &statBuffer) == 0) {
            currentTime = pa4time(NULL);
            entry->idle = (time_t)difftime(currentTime, statBuffer.st_atime);
        } else {
            entry->idle = 0;
        }

        entry->host = (char *)calloc(BUFSIZ, sizeof(char));
        if (entry->host == NULL) {
            perror("calloc");
            freeUtmpInfoTable(*utmpInfoPtr, numElements);
            fclose(utmpFilePtr);
            *utmpInfoPtr = NULL;
            return -1;
        }
        (void)snprintf(entry->host, BUFSIZ, "%.*s", (int)sizeof(utmpxBuf.ut_host) - 1, utmpxBuf.ut_host);

        ++numElements;
    }

    fclose(utmpFilePtr);
    return numElements;
}

void
freeUtmpInfoTable(struct utmpInfo *table, int entries)
{
    int i;

    if (table == NULL) {
        return;
    }

    for (i = 0; i < entries; ++i) {
        free(table[i].host);
    }
    free(table);
}
