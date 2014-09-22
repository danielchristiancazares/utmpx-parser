/*
 * mywho.h
 */

/* Macro Guard */
#ifndef _MYWHO_H
#define _MYWHO_H

#define B_FLAG 0x01             /* -B option: Boot Time */
#define H_FLAG 0x02             /* -H option: Display header */

/* C_FLAG not used in this version. */
#define C_FLAG 0x04             /* -c option: Info about current terminal user */
#define Q_FLAG 0x08             /* -q Quick mywho - only names and # of current users */

                                /*    Ignore all other options */
#define R_FLAG 0x10             /* -r option: Reverse sort */
#define S_FLAG 0x20             /* -S option: Info about select user */
#define SORT_BY_IDLE_FLAG 0x40  /* -i option */
#define SORT_BY_USER_FLAG 0x80  /* -u option */
#define HELP_FLAG 0x100         /* -h option */

/* Extra Credit Options Here */

#define HEADER      "NAME       LINE          IDLE"
#define LONG_HEADER "NAME       LINE         LOGIN         IDLE   PID"

/*
 * Calculate ut_user and ut_line sizes from struct utmpx
 */
#include <utmpx.h>  /* struct utmpx definition */
#include <stddef.h> /* offsetof() macro */

#define USER_SIZE ( offsetof( struct utmpx, ut_id ) - \
                    offsetof( struct utmpx, ut_user ) )

#define LINE_SIZE ( offsetof( struct utmpx, ut_pid ) - \
                    offsetof( struct utmpx, ut_line ) )

/*
 * To pick up pid_t and time_t typedefs
 */
#include <sys/types.h>

struct utmpInfo {
    char   user[USER_SIZE];
    char   line[LINE_SIZE];
    pid_t  pid;
    short  type;
    time_t time;  /* login time.  struct timeval ut_tv; */
                  /* time_t tv_sec member only from struct timeval */
    char  *host;  /* dynamically allocate space for hostname */
    time_t idle;  /* calculated idle time (needed for -i option) */
};

/* Not used in this version.
 * void currentTerminalInfo( struct utmpInfo *utmpTable, int entries );
 */

int buildUtmpInfoTable( const char *utmpFilename,
                        struct utmpInfo **utmpInfoPtr );

void displayUtmpInfo( struct utmpInfo * const table, const int entries,
                      const int displayMode );

void calcIdleTime( time_t idleTime, char *idleString, int size );

/* 
int idleTimeSortAscending( const void *p1, const void *p2 );
int idleTimeSortDescending( const void *p1, const void *p2 );
int nameSortAscending( const void *p1, const void *p2 );
int nameSortDescending( const void *p1, const void *p2 );
*/

time_t pa4time( time_t *t );

/* Other Extra Credit Function Prototypes Here */
/*
int timeSortAscending( const void *p1, const void *p2 );
int timeSortDescending( const void *p1, const void *p2 );
long strToLong( char *str, int base );
void doUptime( struct utmpInfo *utmpTable, int entries );
*/

/* Strings */

#endif /* _MYWHO_H */
