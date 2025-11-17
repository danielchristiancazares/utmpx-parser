#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <string.h>

#include "mywho.h"
#include "strings.h"

int
main( int argc, char **argv )
{
    char fileName[BUFSIZ];

    struct utmpInfo *utmpData = NULL;
    int             numEntries = 0;
    int             cmdInput;
    int             cmdFlag = -1;
   
    while( (cmdInput = getopt( argc, argv, GETOPT_OPTIONS)) != -1 )
    {
        switch( cmdInput )
        {
          case 'h':
              cmdFlag = 0;
              break;

          case 'b':
              cmdFlag = 1;
              break;

          case 'H':
              cmdFlag = 2;
              break;

          case 'q':
              cmdFlag = 3;
              break;

          case 'r':
              cmdFlag = 4;
              break;

          case 'S':
              cmdFlag = 5;
              break;

          case 'i':
              cmdFlag = 6;
              break;

          case 'u':
              cmdFlag = 7;
              break;

          case 'B':
              cmdFlag = 8;
              break;
        }
    }
    if( argc == 3)
    {
        /* Use strncpy to prevent buffer overflow */
        strncpy( fileName, argv[2], BUFSIZ - 1 );
        fileName[BUFSIZ - 1] = '\0';  /* Ensure null termination */
        numEntries = buildUtmpInfoTable( fileName, &utmpData );
    }
    displayUtmpInfo( utmpData, numEntries, cmdFlag );

    /* Clean up allocated memory */
    freeUtmpInfoTable( utmpData, numEntries );

    return 0;
}
