#include <str.h>

int str_cat (int _strs, char* _dest, char** _src)
{
    /* int tmp = 0; */
    int ret    = 0;
    int i      = 0;
    int _bytes = 0;

    if (_strs <= 0)
    {
        ret = 0;
        return ret;
    }

    if (_dest == NULL || _src == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    for (i = 0; i < _strs; i++)
    {
        strcpy (_dest + _bytes, _src [i]);
        _bytes += strlen (_src [i]);
    }
    _dest [_bytes] = '\0';

    /* if (tmp < 0) */
    /* { */
    /*     ret = tmp; */
    /* } */
    return ret;
}

int str_cat_len (int _strs, char** _src)
{
    /* int tmp = 0; */
    int   ret = 0;
    int   i;
    char* _s;

    if (_src == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    for (i = 0; i < _strs; i++)
    {
        _s = _src [i];
        if (_s == NULL)
        {
            ERR_PRINT (_EPTRNULL);
            return -1;
        }
        ret += strlen (_s);
    }
    return ret;
}
