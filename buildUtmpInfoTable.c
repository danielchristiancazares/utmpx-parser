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
    if( *utmpInfoPtr == NULL )
    {
        perror( "calloc for utmpInfo table" );
        fclose( utmpFilePtr );
        exit( 1 );
    }

    struct stat statBuffer;
    time_t accessTime;
    time_t currentTime;
    time_t idleTime;
    char path[BUFSIZ];

    /* Read all of the utmpx structs into the utmpInfoPtr */
    while( fread( &utmpxBuf, sizeof( utmpxBuf ), 1, utmpFilePtr ) == 1 ) {

        /* Check if we've reached the table size limit */
        if( numElements >= tableSize )
        {
            fprintf( stderr, "Warning: utmpx file too large, truncating at %d entries\n", tableSize );
            break;
        }

        strcpy( ((*utmpInfoPtr) + numElements)->user, utmpxBuf.ut_user );
        strcpy( ((*utmpInfoPtr) + numElements)->line, utmpxBuf.ut_line );
        ((*utmpInfoPtr) + numElements)->type = utmpxBuf.ut_type;
        ((*utmpInfoPtr) + numElements)->pid = utmpxBuf.ut_pid;

        /* Get current time first */
        currentTime = pa4time( &currentTime );

        /* Initialize accessTime to current time (0 idle if stat fails) */
        accessTime = currentTime;

        strcpy( path, "/dev/");
        strcat( path, utmpxBuf.ut_line );
        if( stat( path, &statBuffer) == 0)
        {
            accessTime = statBuffer.st_atime;
        }

        idleTime = (time_t) difftime( currentTime, accessTime );

        /* Store login time and idle time in the correct fields */
        ((*utmpInfoPtr) + numElements)->time = utmpxBuf.ut_tv.tv_sec;
        ((*utmpInfoPtr) + numElements)->idle = idleTime;
        ((*utmpInfoPtr) + numElements)->host = (char *) calloc( BUFSIZ, sizeof( char ) );
        if( ((*utmpInfoPtr) + numElements)->host == NULL )
        {
            perror( "calloc for hostname" );
            fclose( utmpFilePtr );
            freeUtmpInfoTable( *utmpInfoPtr, numElements );
            exit( 1 );
        }
        strcpy( ((*utmpInfoPtr) + numElements)->host, utmpxBuf.ut_host );

        ++numElements;

    }

    /* Close the file */
    fclose( utmpFilePtr );

    return numElements;
}

/*
 * Free all dynamically allocated memory for the utmpInfo table
 */
void
freeUtmpInfoTable( struct utmpInfo *table, int entries )
{
    int i;

    if( table == NULL )
        return;

    /* Free each dynamically allocated hostname string */
    for( i = 0; i < entries; i++ )
    {
        if( table[i].host != NULL )
        {
            free( table[i].host );
        }
    }

    /* Free the table itself */
    free( table );
}
