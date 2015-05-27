/**
@file err.c
@author Roger Bongers
@date May 27 2015
@see err.h
**/
#include <err.h>

/**
@hideinitializer
**/
const err_info_t err_table [] =
{
#include <data/err_table.h>
    { 0, "\0" }
};

int       err_num = _ESUCCESS;
const int ERR_MAX = sizeof (err_table) / sizeof (err_info_t) - 1;

void _err_pr (FILE* _fp, char* _src, const char* _fn, int _l, char* _fmt, ...)
{
    // int tmp = 0;
    // int ret = 0;
    va_list _args = { 0 };

    fprintf (_fp, "%s:%s:%s():%d: ", "mm", _src, _fn, _l);
    va_start (_args, _fmt);
    vfprintf (_fp, _fmt, _args);
    va_end (_args);
    fprintf (_fp, "\n");

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return;
}

char* _err_get_msg (int _code)
{
    // int tmp = 0;
    char* ret = 0;

    if (_code >= ERR_MAX || _code < 0)
    {
        ret = err_table [_EUNKNOWN].msg;
    }
    else
    {
        ret = err_table [_code].msg;
    }

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

void err_reset (void)
{
    // int tmp = 0;
    // int ret = 0;

    err_num = 0;
    errno   = 0;

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return;
}
