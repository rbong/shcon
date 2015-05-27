/**
@file err.h
@author Roger Bongers
@date May 27 2015
@brief Error processing.

Defines a system for printing and recording errors based on error codes.

New error codes should be entered into #err_table along with their messages.

To print error codes defined by #err_table, use the ERR_PRINT() macro.
To print error codes defined by the system, use the ERR_SYS() macro.
**/
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <error.h>
#include <string.h>

/**
@brief Prints program error messages.

Calls _err_pr() to print error from #err_table. Also sets #err_num.
The location printed by _err_pr() will be where the macro is called.
@param n Refers to the program error message to print and save.
**/
#define ERR_PRINT(n) (_err_pr \
        (stderr, __FILE__, __func__, __LINE__, _err_get_msg (err_num = n)))
/**
@brief Prints system error messages.

Calls _err_pr() to print a system error.
The location printed by _err_pr() will be where the macro is called.
@param n Refers to the system error message to print.
**/
#define ERR_SYS(n) (_err_pr \
        (stderr, __FILE__, __func__, __LINE__, strerror (n)))

/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_ERR
#define MM_ERR
/**
@brief Contains error information.

Used by #err_table to provide an index of error messages.
**/
typedef struct
{
    int   code; //!< The error number.
    char* msg; //!< The error message.
} err_info_t;

/**
@brief Assigns numbers to error codes.

At compile time, err_codes is populated with err_codes.h.
@see
err_table
*/
enum err_codes
{
#include <data/err_codes.h>
};
/**
@file err_codes.h
@brief The raw error code numbers.

This is where all error codes are defined for the compiler.
To see a description of each error code, see the source for err_table.h.

While this is used by #err_table to index errors at run time, it is actually
generated using the information that is in err_table.h before compile time.

The test system ensures consistency of error codes.
@see err_codes err_table err_table.h
**/

/**
@brief The index of error codes and messages.

Any new error messages should be entered into err_table.h, which populates
\a err_table at compile time.

The test system ensures consistency of error codes.
@see
err_table.h err_info_t
**/
extern const err_info_t err_table [];
/**
@file data/err_table.h
@brief The raw index of error codes.

This is where all error codes are first created by the writer.
Before compiling, err_table.h is used to generate err_codes.h with ecodegen.sh.

The test system ensures consistency of error codes.

Follow the existing style to enter in new error messages.
_ESUCCESS should always be the first value.
_EUNKNOWN should never be erased.
@see err_codes.h
**/

/**
@brief The last error that occured.

This is only set by ERR_PRINT() and err_reset().
**/
extern int       err_num;
/**
@brief The maximum error number that is defined.
**/
extern const int ERR_MAX;
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

/**
@brief Prints location and error.

This function prints in the style of program:filename:function():line: message.
@param _fp The file stream to print to.
@param _src The source file name.
@param _fn The function name.
@param _l The line number.
@param _fmt The format to print the rest of the arguments, or a plain string.
**/
void _err_pr (FILE* _fp, char* _src, const char* _fn, int _l, char* _fmt, ...);
/**
@brief Retrieves an error message.
@param _code The error code corresponding to the message.
@see err_info_t
**/
char* _err_get_msg (int _code);
/**
@brief Sets program and system error variables to their default values.
**/
void err_reset (void);
