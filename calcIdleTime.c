#include <time.h>
#include <stdio.h>
#include <string.h>

void
calcIdleTime( time_t idleTime, char *idleString, int size )
{
    double seconds, minutes, hours;
    seconds =  idleTime;
    
    if( seconds < (double) 60.0 )
    {
        strcpy( idleString, " . " );
    }
    else if( seconds > (double) 86400.0 )
    {
        strcpy( idleString, "old" );
    }
    else
    {
        minutes = seconds / (double) 60.0;
        seconds -= minutes * (double) 60.0;
    
        hours   = minutes / (double) 60.0 ;
        minutes -= hours * (double) 60.0;
   
        snprintf( idleString, size, "%.f:%.f", hours, minutes );
    }
}
