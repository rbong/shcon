// todo- overhaul error framework
#include <err.h>

const err_info_t err_table [] =
{
#include <data/err_table.h>
    { 0, "\0" }
};

int       err_num = _ESUCCESS;
const int ERR_MAX = sizeof (err_table) / sizeof (err_info_t) - 1;

void _err_pr (FILE* fp, char* _file, const char* _fn, int _ln, char* _fmt, ...)
{
    va_list _args = { 0 };

    fprintf (fp, "%s:%s:%s():%d: ", "mm", _file, _fn, _ln);
    va_start (_args, _fmt);
    vfprintf (fp, _fmt, _args);
    va_end (_args);
    fprintf (fp, "\n");

    return;
}

void _err_sys (FILE* fp, char* _file, const char* _fn, int _ln, int n)
{
    fprintf (fp, "%s:%s:%s():%d: ", "mm", _file, _fn, _ln);
    fprintf (fp, "System: %s.\n", strerror (n));

    return;
}

char* _err_s_msg (int _code)
{
    if (_code >= ERR_MAX || _code < 0)
    {
        return err_table [_EUNKNOWN].msg;
    }

    return err_table [_code].msg;
}

void err_reset (void)
{
    err_num = 0;
    errno   = 0;

    return;
}
