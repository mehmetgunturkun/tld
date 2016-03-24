#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>

inline int println(const char *format, ...) {
    va_list arg;
    int done;

    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    printf("\n");
    va_end (arg);
    return done;
}

#endif
