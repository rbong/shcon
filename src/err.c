// todo- use err_reset on every function
#include <err.h>

const err_info_t err_table [] =
{
#include <data/err_table.h>
    { 0, "\0" }
};

int       err_number = _ESUCCESS;
const int ERR_MAX    = sizeof (err_table) / sizeof (err_info_t) - 1;

void err_set (int code)
{
    err_number = code;
}

char* err_get_msg (int code)
{
    if (code >= ERR_MAX || code < 0)
    {
        return err_table [_EUNKNOWN].msg;
    }
    return err_table [code].msg;
}

void err_reset (void)
{
    err_number = 0;
    errno = 0;
    return;
}
