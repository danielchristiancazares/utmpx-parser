#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <utmpx.h>
#include <sys/stat.h>
#include <string.h>

#include "mywho.h"

int
buildUtmpInfoTable( const char *utmpFilename, struct utmpInfo **utmpInfoPtr )
{
    FILE          *utmpFilePtr;
    struct utmpx  utmpxBuf;

    /* Open the file if it exists */
    errno = 0;
    utmpFilePtr = fopen( utmpFilename, "r" );
    if( utmpFilePtr == NULL )
    {
	      perror( utmpFilename );
	      exit(1);
    }

    /*  Define an initial table size of 10 elements. */
    int tableSize   = 500,
        numElements = 0;
    
    /* Allocate memory for the initial table */
    *utmpInfoPtr = (struct utmpInfo *) calloc( tableSize, sizeof( struct utmpInfo ) );

    struct stat statBuffer;
    time_t accessTime;
    time_t currentTime;
    time_t idleTime;
    char path[BUFSIZ];

    /* Read all of the utmpx structs into the utmpInfoPtr */
    while( fread( &utmpxBuf, sizeof( utmpxBuf ), 1, utmpFilePtr ) == 1 ) {
        
        strcpy( ((*utmpInfoPtr) + numElements)->user, utmpxBuf.ut_user );
        strcpy( ((*utmpInfoPtr) + numElements)->line, utmpxBuf.ut_line );
        ((*utmpInfoPtr) + numElements)->type = utmpxBuf.ut_type;
        ((*utmpInfoPtr) + numElements)->pid = utmpxBuf.ut_pid;
        
        strcpy( path, "/dev/");
        strcat( path, utmpxBuf.ut_line );
        if( stat( path, &statBuffer) == 0)
        {
            accessTime = statBuffer.st_atime;
        }

        currentTime = pa4time( &currentTime );
        idleTime = (time_t) difftime( currentTime, accessTime );

        ((*utmpInfoPtr) + numElements)->time = idleTime; 
        ((*utmpInfoPtr) + numElements)->host = (char *) calloc( BUFSIZ, sizeof( char ) ); 
        strcpy( ((*utmpInfoPtr) + numElements)->host, utmpxBuf.ut_host );

        ++numElements;

    }
    
    return 0; 
}
