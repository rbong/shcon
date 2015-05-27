/**
@file err.h
@author Roger Bongers
@date May 27 2015
@brief Error processing.

Defines a system for printing and recording errors based on error codes.

New error codes should be entered into err_table.h along with their messages.
After entering in new error codes, run the ecodegen.sh script, or run make.
New error codes will be available after recompiling.
Do not manually enter codes into #err_codes.

To print error codes defined by err_table.h, use the ERR_PRINT() macro.
To print error codes defined by the system, use the ERR_SYS() macro.
**/
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <error.h>
#include <string.h>

/**
@def ERR_PRINT(n)
@brief Prints program error messages.

A macro.

Calls _err_pr() to print error from #err_table. Also sets #err_num.
The location printed by _err_pr() will be the location that calls the macro.
@param[in] n Refers to the program error message to print. Saved in #err_num.
@return None.
**/
#define ERR_PRINT(n) \
    (_err_pr (stderr, __FILE__, __func__, __LINE__, _err_s_msg (err_num = n)))
/**
@def ERR_SYS(n)
@param[in] n The error code to print.
@brief Prints system error messages.

A macro.

Calls _err_pr() to print a system error.
The location printed by _err_pr() will be the location that calls the macro.
**/
#define ERR_SYS(n) \
    (_err_pr (stderr, __FILE__, __func__, __LINE__, strerror (n)))

/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_ERR
#define MM_ERR
typedef struct
{
    int   code;
    char* msg;
} err_info_t;

// err_codes- custom error numbers generated from the ecodegen.sh script.
enum err_codes
{
#include <data/err_codes.h>
};

// err_info_t- indexes custom error numbers, used to generate err_codes.
extern const err_info_t err_table [];

// err_num- the last custom error that occured.
extern int       err_num;
// ERR_MAX- the largest error number that is defined.
extern const int ERR_MAX;
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

/*
 * _err_pr ()- called by ERR_PRINT () and ERR_SYS () to print errors.
 * args-       file stream, file name, function name, line number, format, ...
 * returns-    returns no value and does not set err_num.
 */
void _err_pr (FILE*, char*, const char*, int, char*, ...);
/*
 * _err_s_msg ()- called by the ERR_PRINT macro to obtain custom error strings.
 * args-          error enum
 * returns-       a pointer to the error message specified by the error enum.
 */
char* _err_s_msg (int);
/*
 * err_reset ()- zeros out system and local error number.
 * args-         none
 * returns-      returns no value and does not set err_num.
 */
void err_reset (void);
