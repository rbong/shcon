#include <str.h>

int str_cat (int _strs, char* _dest, char** _src)
{
    /* int tmp   = 0; */
    int ret   = 0;
    int i     = 0;
    int bytes = 0;

    if (_strs <= 0)
    {
        ret = 0;
        return ret;
    }

    if (_dest == NULL || _src == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
        return ret;
    }

    for (i = 0; i < _strs; i++)
    {
        strcpy (_dest + bytes, _src [i]);
        bytes += strlen (_src [i]);
    }
    _dest [bytes] = '\0';

    /* if (tmp < 0) */
    /* { */
    /*     ret = tmp; */
    /* } */
    return ret;
}

int str_cat_len (int _strs, char** _src)
{
    /* int tmp = 0; */
    int ret = 0;
    int i;
    char* s;

    if (_src == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    for (i = 0; i < _strs; i++)
    {
        s = _src [i];
        if (s == NULL)
        {
            ERR_PRINT (_EPTRNULL);
            return -1;
        }
        ret += strlen (s);
    }
    return ret;
}
