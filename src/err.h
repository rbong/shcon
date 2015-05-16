#include <error.h>
#include <errno.h>

#define ERROR_AT_LINE(status, ...) \
    (error_at_line (status, 0, __FILE__, __LINE__, __VA_ARGS__))
#define ERROR_AT_LINE_SYS(status, errnum) \
    (error_at_line (status, errnum, __FILE__, __LINE__, "System"))
#define ERROR_PRINT(status) \
    (error_at_line (status, 0, __FILE__, __LINE__, \
     "%s", error_get_msg (error_number)))

#ifndef MM_ERR
#define MM_ERR
typedef struct error_info_t
{
    int   code;
    char* msg;
} error_info_t;

enum error_codes
{
// todo- proper include directories
#include <data/error_codes.h>
};

extern const error_info_t error_table [];

extern int       error_number;
extern const int ERROR_MAX;
#endif

void      error_set     (int);
char*     error_get_msg (int);
void      err_reset     (void);
