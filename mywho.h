/*
 * mywho.h
 */

#ifndef _MYWHO_H
#define _MYWHO_H

#define B_FLAG 0x01
#define H_FLAG 0x02
#define Q_FLAG 0x08
#define HELP_FLAG 0x100

#define HEADER      "NAME       LINE          IDLE"

#include <sys/types.h>
#include <utmpx.h>

#define USER_SIZE (sizeof(((struct utmpx *)0)->ut_user))
#define LINE_SIZE (sizeof(((struct utmpx *)0)->ut_line))

struct utmpInfo {
    char user[USER_SIZE];
    char line[LINE_SIZE];
    pid_t pid;
    short type;
    time_t time;
    char *host;
    time_t idle;
};

int buildUtmpInfoTable(const char *utmpFilename, struct utmpInfo **utmpInfoPtr);
void freeUtmpInfoTable(struct utmpInfo *table, int entries);

void displayUtmpInfo(struct utmpInfo *table, int entries, int displayMode);

void calcIdleTime(time_t idleTime, char *idleString, int size);

int idleTimeSortAscending(const void *p1, const void *p2);
int idleTimeSortDescending(const void *p1, const void *p2);
int nameSortAscending(const void *p1, const void *p2);
int nameSortDescending(const void *p1, const void *p2);
int timeSortAscending(const void *p1, const void *p2);
int timeSortDescending(const void *p1, const void *p2);
long strToLong(char *str, int base);
void doUptime(struct utmpInfo *utmpTable, int entries);
void currentTerminalInfo(struct utmpInfo *utmpTable, int entries);

time_t pa4time(time_t *t);

#endif
