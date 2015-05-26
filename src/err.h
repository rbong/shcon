#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <error.h>
#include <string.h>

#define ERR_PRINT(n) \
    (_err_pr (stderr, __FILE__, __func__, __LINE__, _err_s_msg (err_num = n)))
#define ERR_SYS(n) \
    (_err_sys (stderr, __FILE__, __func__, __LINE__, n))

/* -- START OF GUARD BLOCK -- */
#ifndef MM_ERR
#define MM_ERR
typedef struct
{
    int   code;
    char* msg;
} err_info_t;

enum err_codes
{
#include <data/err_codes.h>
};

extern const err_info_t err_table [];

extern int       err_num;
extern const int ERR_MAX;
#endif
/* --- END OF GUARD BLOCK --- */

void  _err_pr    (FILE*, char*, const char*, int, char*, ...);
void  _err_sys   (FILE*, char*, const char*, int, int);
char* _err_s_msg (int);
void  err_reset  (void);
