#ifndef DEBUG_H
#ifndef DEBUG_H_INT
#ifndef DEBUG_H_STRING
#ifndef DEBUG_H_DOUBLE
#define DEBUG_H

#include <stdio.h> /* fprintf(), stderr,*/
#include <string.h> /*str*/

/* defines a debug mode variable that exists in another file (debug.c) */
extern int debug_mode; 


/*in the main, if runtime flag is -d, debug_mode = 1, else debug_mode = 0.
Use these these conditions to create toggleable macros */

/*prints a message in debug mode*/
#define DEBUG_PRINT(fmt, ...)\
    do { \
        if (debug_mode) { \
            fprintf(stderr, fmt); \
            } \
        } while (0);

#endif

/*prints an integer in debug mode*/
#define DEBUG_PRINT_INT(fmt, int_arg)\
    do { \
        if (debug_mode) { \
            fprintf(stderr, fmt, int_arg); \
        } \
    } while (0)
#endif

/*prints a string in debug mode*/
#define DEBUG_PRINT_STRING(fmt, str_arg)\
    do { \
        if (debug_mode) { \
            fprintf(stderr, fmt, str_arg); \
        } \
    } while (0)
#endif

/*prints a double in debug mode*/
#define DEBUG_PRINT_DOUBLE(fmt, double_arg)\
    do { \
        if (debug_mode) { \
            fprintf(stderr, fmt, double_arg); \
        } \
    } while (0)
#endif
