#include <error.h>
#include <errno.h>

#define ERR_AT_LINE(status, ...) \
    (error_at_line (status, 0, __FILE__, __LINE__, __VA_ARGS__))
#define ERR_AT_LINE_SYS(status, errnum) \
    (error_at_line (status, errnum, __FILE__, __LINE__, \
     "%s(): System", __func__))
#define ERR_PRINT(status) \
    (error_at_line (status, 0, __FILE__, __LINE__, \
     "%s(): %s", __func__, err_get_msg (err_number)))

#ifndef MM_ERR
#define MM_ERR
typedef struct err_info_t
{
    int   code;
    char* msg;
} err_info_t;

enum err_codes
{
// todo- proper include directories
#include <data/err_codes.h>
};

extern const err_info_t err_table [];

extern int       err_number;
extern const int ERR_MAX;
#endif

void      err_set     (int);
char*     err_get_msg (int);
void      err_reset     (void);
