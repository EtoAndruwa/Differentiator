#ifndef DEBUGGER_H
#define DEBUGGER_H

/*####################################################################################################################################################################*/

#define CONSOLE_RED   "\033[1;31m" // Makes the text red
#define CONSOLE_CLEAR "\033[0m"    // Clears the text (sets color to the default one)

/**
 * @brief This macro is used in order to display error message in stderr or another stream
 */
#define ERROR_MESSAGE(file, ERROR_CODE)                                                                              \                                                                                     
    fprintf(file, "\n\n%sERROR: %d (%s). The error called from file: %s, from function: %s, from line %d %s\n\n\n",  \
            CONSOLE_RED, ERROR_CODE, #ERROR_CODE, __FILE__, __func__, __LINE__, CONSOLE_CLEAR);                      \
            
/*####################################################################################################################################################################*/

#endif
