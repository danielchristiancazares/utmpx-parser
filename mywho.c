#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "mywho.h"
#include "strings.h"

static int parseFlags(int argc, char **argv);

int
main(int argc, char **argv)
{
    struct utmpInfo *utmpData = NULL;
    const char *fileName = NULL;
    int cmdFlag;
    int entries;

    cmdFlag = parseFlags(argc, argv);

    if (cmdFlag == HELP_FLAG) {
        displayUtmpInfo(NULL, 0, cmdFlag);
        return EXIT_SUCCESS;
    }

    if (optind >= argc) {
        (void)fprintf(stderr, "Usage: mywho %s", SHORT_USAGE);
        return EXIT_FAILURE;
    }

    fileName = argv[optind];
    entries = buildUtmpInfoTable(fileName, &utmpData);
    if (entries < 0) {
        return EXIT_FAILURE;
    }

    displayUtmpInfo(utmpData, entries, cmdFlag);
    freeUtmpInfoTable(utmpData, entries);

    return EXIT_SUCCESS;
}

static int
parseFlags(int argc, char **argv)
{
    int cmdInput;
    int cmdFlag = 0;

    while ((cmdInput = getopt(argc, argv, GETOPT_OPTIONS)) != -1) {
        switch (cmdInput) {
        case 'h':
            cmdFlag |= HELP_FLAG;
            break;
        case 'B':
            cmdFlag |= B_FLAG;
            break;
        case 'H':
            cmdFlag |= H_FLAG;
            break;
        case 'q':
            cmdFlag |= Q_FLAG;
            break;
        default:
            (void)fprintf(stderr, "Usage: mywho %s", SHORT_USAGE);
            exit(EXIT_FAILURE);
        }
    }

    return cmdFlag;
}
