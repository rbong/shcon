#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <error.h>
#include <string.h>

/*
 * ERR_PRINT ()- prints custom error messages.
 * args-         error number
 * returns-      returns no value and does not set err_num.
 */
#define ERR_PRINT(n) \
    (_err_pr (stderr, __FILE__, __func__, __LINE__, _err_s_msg (err_num = n)))
/* ERR_SYS ()- prints system error messages.
 * args-       errno
 * returns-    returns no value and does not set err_num.
 */
#define ERR_SYS(n) \
    (_err_pr (stderr, __FILE__, __func__, __LINE__, strerror (n)))

/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_ERR
#define MM_ERR
// err_info_t- contains custom error information for indexing and printing.
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
