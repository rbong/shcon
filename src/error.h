#define ERROR_AT_LINE(status, errnum, ...) \
    (error_at_line (status, errnum, __FILE__, __LINE__, __VA_ARGS__))
#define ERROR_PRINT(status) \
    (error_at_line (status, 0, __FILE__, __LINE__, \
     "%s", error_get_msg (error_number)))

typedef struct error_info_t
{
    int   code;
    char* msg;
} error_info_t;

enum error_codes
{
// todo- proper include directories
#include "../include/data/error_codes.h"
};

const error_info_t error_table [] =
{
#include "../include/data/error_table.h"
    { 0, "\0" }
};

int       error_number = _ESUCCESS;
const int ERROR_MAX    = sizeof (error_table) / sizeof (error_info_t) - 1;

void      error_set     (int);
char*     error_get_msg (int);

#ifndef MM_ERROR
#define MM_ERROR
#include "error.c"
#endif
