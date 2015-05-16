#include <err.h>

// todo- rename error_ to err_

const error_info_t error_table [] =
{
#include <data/error_table.h>
    { 0, "\0" }
};

int       error_number = _ESUCCESS;
const int ERROR_MAX    = sizeof (error_table) / sizeof (error_info_t) - 1;

void error_set (int code)
{
    error_number = code;
}

char* error_get_msg (int code)
{
    if (code >= ERROR_MAX || code < 0)
    {
        return error_table [_EUNKNOWN].msg;
    }
    return error_table [code].msg;
}

void err_reset (void)
{
    error_number = 0;
    errno = 0;
    return;
}
