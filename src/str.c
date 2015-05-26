#include <str.h>

int str_cat (int _strs, char* _dest, char** _src)
{
    int i;
    int bytes = 0;

    if (_strs <= 0)
    {
        return 0;
    }

    if (_dest == NULL || _src == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    for (i = 0; i < _strs; i++)
    {
        strcpy (_dest + bytes, _src [i]);
        bytes += strlen (_src [i]);
    }
    _dest [bytes] = '\0';

    return 0;
}

int str_cat_len (int _strs, char** _src)
{
    int i;
    int len = 1;
    char* s;

    if (_src == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    for (i = 0; i < _strs; i++)
    {
        s = _src [i];
        if (s == NULL)
        {
            ERR_PRINT (_EPTRNULL);
            return -1;
        }
        len += strlen (s);
    }
    return len;
}
