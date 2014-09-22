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
    if( displayMode == 0 )
    {
        (void) fprintf( stderr, LONG_USAGE);
        exit ( 1 );
    }
    if( displayMode == 8 )
    {
        if ( strftime( timeStr, sizeof( timeStr ), "%b %e %H:%M",
	                             localtime( &(table[1].time) ) ) == 0 )
        {
	          (void) fprintf( stderr, "Problems with strftime()\n" );
	          exit( 1 );
        }
        (void) fprintf( stdout, "%4s %17s %13s \n", ".", table[1].line, timeStr );
    }
    if( displayMode == 2 )
    {
        (void) fprintf( stdout, "%-10s %-10s %7s \n", "NAME", "LINE", "IDLE" ); 
        while( i < 500 )
        {
            if( table[ i ].type == USER_PROCESS )
            {
                if ( strftime( timeStr, sizeof( timeStr ), "%Y-%m-%d %H:%M:%S",
	                             localtime( &(table[i].time) ) ) == 0 )
                {
	                  (void) fprintf( stderr, "Problems with strftime()\n" );
	                  exit( 1 );
	              }

                calcIdleTime( table[i].time, timeStr, BUFSIZ );  
                (void) fprintf( stdout, "%-10s %-10s %7s   (%s)\n", table[i].user,
                                                             table[i].line,
                                                             timeStr, 
                                                             table[i].host ); 
            }
            ++i;
        }
    }
    if( displayMode == 3 )
    {
        int totalItems = 0;
        while( i < entries )
        {
            if( table[ i ].type == USER_PROCESS )
            {
                (void) fprintf( stdout, "%-8s ", table[ i ].user );
                ++totalItems;
                if(totalItems % 8 == 0)
                {
                    (void) fprintf( stdout, "\n" );
                }
            }
            ++i;
        }
        (void) fprintf( stdout, "\n");
        (void) fprintf( stdout, NUM_USERS_FMT, totalItems );
    }
}
