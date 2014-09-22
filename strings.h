/*
 * Filename: strings.h
 * Author: Daniel Cazares
 * Userid: cs30xav
 * Description: Strings used in PA4 output
 */


#define FOPEN_ERR "mywho: Cannot open file \"%s\""
#define REALLOC_ERR "realloc() in readUtmpInfo"
#define CALLING_PA4TIME_ERR "buildUtmpInfoTable() calling time()"

#define FILE_NOT_UTMPX "mywho: File \"%s\" is not a utmpx file\n"

#define DEV_FMT "/dev/"

#define BLANK_FMT "     "
#define DOT_FMT "  .  "
#define OLD_FMT " old "
#define IDLE_TIME_FMT "%2d:%02d"

#define USER_NAME_FMT "%-8s%c"
#define NUM_USERS_FMT "# users=%d\n"
#define HEADER_FMT "%s\n"
#define STRFTIME_FMT "%b %e %H:%M"
#define STRFTIME_ERR "Problems with strftime()\n"
#define USER_NOT_FOUND_ERR "User \"%s\" not found.\n"
#define LONG_DISPLAY_T_FMT "%-8s %c %-12s %s %s %6ld\t(%s)\n"
#define LONG_DISPLAY_FMT "%-8s %c %-12s %s\t(%s)\n"

#define SHORT_USAGE "[-aBbHhqrST[i|t|u]] utmpx_like_file\n" \
	"        mywho -q [-n x] utmpx_like_file\n" \
        "            default output: name, line, idle time, hostname\n\n"

#define LONG_USAGE "[-aBbHhqrST[i|t|u]] utmpx_like_file\n" \
        "        mywho -q [-n x] utmpx_like_file\n" \
        "            default output: name, line, idle time, hostname\n\n" \
        "h       this long help message\n" \
        "B       print boot time only\n" \
        "H       print header\n" \
        "q       quick mywho info only\n" \
        "r       reverse meaning of any sort option\n" \
        "            ignored if no sort option specified\n" \
        "S user  output all info on this username only\n" \
        "i       output sorted by idle time (ascending)\n" \
        "u       output sorted by username (ascending)\n" \
        "            only one of t or u or i allowed (mutual exclusion)\n" \
        "\nExtra Credit Options:\n" \
        "t       output sorted by login time (ascending) [Extra Credit]\n" \
        "            only one of t or u or i allowed (mutual exclusion)\n" \
        "n x     specify number of users per line for -q [Extra Credit]\n" \
        "b       output uptime-like info [Extra Credit]\n" \
        "T       add to default output [Extra Credit]\n" \
        "            status of line (+ writable, - not writable, ? unknown),\n"\
        "            login time, and pid\n" \
        "a       all (BbHT options) [Extra Credit]\n"

#define GETOPT_OPTIONS "aBbHhin:qrS:Ttu"
#define GETOPT_ERR "Cannot specify more than one of -i, -u, and -t\n"
#define NUM_USERS_ERR "%s: Number of users per line must be at least 1\n"
#define USAGE_FMT "\nUsage:  %s %s\n"

#define STR_NEWLINE "\n"
