#ifndef DEBUGGER_H
#define DEBUFFER_H

#define CONSOLE_RED   "\\033[1;31m"
#define CONSOLE_CLEAR "\\033[0m"

#define ERROR_MESSAGE(file, ERROR_CODE)                                                                                                                   \
    fprintf(file, CONSOLE_RED, "ERROR: %ld (%s) ", CONSOLE_CLEAR, "The error called from file: %s, from function: %s, from line %d", ERROR_CODE, #ERROR_CODE, __FILE__, __func__, __LINE__); \

#endif
