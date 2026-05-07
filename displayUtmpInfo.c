#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utmpx.h>

#include "mywho.h"
#include "strings.h"

void
displayUtmpInfo(struct utmpInfo *table, int entries, int displayMode)
{
    char idleStr[BUFSIZ];
    char timeStr[BUFSIZ];
    int i;
    int totalItems = 0;

    if ((displayMode & HELP_FLAG) != 0) {
        (void)fprintf(stdout, "%s", LONG_USAGE);
        return;
    }

    if ((displayMode & B_FLAG) != 0) {
        for (i = 0; i < entries; ++i) {
            if (table[i].type == BOOT_TIME) {
                if (strftime(timeStr, sizeof(timeStr), "%b %e %H:%M", localtime(&(table[i].time))) == 0) {
                    (void)fprintf(stderr, "Problems with strftime()\n");
                    return;
                }
                (void)fprintf(stdout, "%4s %17s %13s \n", ".", table[i].line, timeStr);
                return;
            }
        }
    }

    if ((displayMode & Q_FLAG) != 0) {
        for (i = 0; i < entries; ++i) {
            if (table[i].type == USER_PROCESS) {
                (void)fprintf(stdout, "%-8s ", table[i].user);
                ++totalItems;
                if (totalItems % 8 == 0) {
                    (void)fprintf(stdout, "\n");
                }
            }
        }
        (void)fprintf(stdout, "\n");
        (void)fprintf(stdout, NUM_USERS_FMT, totalItems);
        return;
    }

    if ((displayMode & H_FLAG) != 0) {
        (void)fprintf(stdout, "%-10s %-10s %7s \n", "NAME", "LINE", "IDLE");
    }

    for (i = 0; i < entries; ++i) {
        if (table[i].type == USER_PROCESS) {
            calcIdleTime(table[i].idle, idleStr, BUFSIZ);
            (void)fprintf(stdout, "%-10s %-10s %7s   (%s)\n", table[i].user,
                          table[i].line, idleStr, table[i].host);
        }
    }
}
