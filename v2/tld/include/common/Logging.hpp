#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <stdlib.h>

#include "common/Colors.hpp"

#define DEBUG_LEVEL 1
#define WARN_LEVEL 2
#define ERROR_LEVEL 3

#define LOG_LEVEL WARN_LEVEL

#if LOG_LEVEL <= DEBUG_LEVEL
    #define DEBUG(fmt, ...) printf(fmt, ##__VA_ARGS__); printf("\n");
#else
    #define DEBUG(fmt, ...)
#endif

#if LOG_LEVEL <= DEBUG_LEVEL
    #define DEBUGALL(body) body
#else
    #define DEBUGALL(body)
#endif

#if LOG_LEVEL <= WARN_LEVEL
    #define WARN(fmt, ...) printf(fmt, ##__VA_ARGS__); printf("\n");
#else
    #define WARN(fmt, ...)
#endif

#if LOG_LEVEL <= ERROR_LEVEL
    #define ERROR(fmt, ...) printf(fmt, ##__VA_ARGS__); printf("\n");
#else
    #define ERROR(fmt, ...)
#endif

#if LOG_LEVEL <= ERROR_LEVEL
    #define ERRORALL(body) body
#else
    #define ERRORALL(body)
#endif

#define println(format, ...) printf(format, ##__VA_ARGS__); printf("\n");

#endif
