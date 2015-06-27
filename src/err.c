/**
@file err.c
@author Roger Bongers
@date May 27 2015
@brief See err.h.
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
int err_verbose = 1;

void _err_pr (FILE* _fp, err_loc_t _loc, char* _fmt, ...)
{
    // int tmp = 0;
    // int ret = 0;
    va_list _args = { 0 };

    fprintf (_fp, "mm:");
    if (_loc.file != NULL)
    {
        fprintf (_fp, "%s:", _loc.file);
    }
    if (_loc.func != NULL)
    {
        fprintf (_fp, "%s():", _loc.func);
    }
    if (_loc.line > 0)
    {
        fprintf (_fp, "%d:", _loc.line);
    }
    fprintf (_fp, " ");
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

void _err_from (FILE* _fp, err_loc_t _loc)
{
    // int tmp = 0;
    // int ret = 0;

    if (err_verbose)
    {
        fprintf (_fp, "from mm:");
        if (_loc.file != NULL)
        {
            fprintf (_fp, "%s:", _loc.file);
        }
        if (_loc.func != NULL)
        {
            fprintf (_fp, "%s():", _loc.func);
        }
        if (_loc.line > 0)
        {
            fprintf (_fp, "%d\n", _loc.line);
        }
    }

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    // return ret;
}
