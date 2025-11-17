#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <utmpx.h>

#include "mywho.h"
#include "strings.h"
void
displayUtmpInfo( struct utmpInfo * const table, const int entries, const int displayMode )
{
    char timeStr[BUFSIZ];
    int i = 0;
    int totalItems;

    switch( displayMode )
    {
        case 0:  /* Help */
            (void) fprintf( stderr, LONG_USAGE);
            exit( 1 );
            break;

        case 8:  /* Boot time */
            if ( strftime( timeStr, sizeof( timeStr ), "%b %e %H:%M",
                           localtime( &(table[1].time) ) ) == 0 )
            {
                (void) fprintf( stderr, "Problems with strftime()\n" );
                exit( 1 );
            }
            (void) fprintf( stdout, "%4s %17s %13s \n", ".", table[1].line, timeStr );
            break;

        case 2:  /* Header mode */
            (void) fprintf( stdout, "%-10s %-10s %7s \n", "NAME", "LINE", "IDLE" );
            for( i = 0; i < entries; i++ )
            {
                if( table[i].type == USER_PROCESS )
                {
                    /* Calculate idle time directly - no need for strftime */
                    calcIdleTime( table[i].idle, timeStr, BUFSIZ );
                    (void) fprintf( stdout, "%-10s %-10s %7s   (%s)\n",
                                   table[i].user, table[i].line,
                                   timeStr, table[i].host );
                }
            }
            break;

        case 3:  /* Quick mode */
            totalItems = 0;
            for( i = 0; i < entries; i++ )
            {
                if( table[i].type == USER_PROCESS )
                {
                    (void) fprintf( stdout, "%-8s ", table[i].user );
                    ++totalItems;
                    if( totalItems % 8 == 0 )
                    {
                        (void) fprintf( stdout, "\n" );
                    }
                }
            }
            (void) fprintf( stdout, "\n");
            (void) fprintf( stdout, NUM_USERS_FMT, totalItems );
            break;

        default:
            /* No output for unimplemented modes */
            break;
    }
}
