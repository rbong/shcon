#include <str.h>

int str_cat (int nstr, char* dest, char** src)
{
    int i;
    int len;
    int nbyte = 0;

    if (dest == NULL || src == NULL)
    {
        set_error (_EPTRNULL);
        return -1;
    }
    len = str_cat_ln (nstr, src);
    if (len < 0)
    {
        return len;
    }
    if (size (dest) < len)
    {
        set_error (_EPTROVERFLOW);
        return -1;
    }

    for (i = 0; i < nstr; i++)
    {
        strcpy (dest + nbyte, src [i]);
        nbyte += strlen (src [i]);
    }
    dest [len - 1] = '\0';

    return 0;
}

int str_cat_len (int nstr, char** src)
{
    int i;
    int len = 1;
    char* s;

    if (src == NULL)
    {
        set_error (_EPTRNULL);
        return -1;
    }

    for (i = 0; i < nstr; i++)
    {
        s = str [i];
        if (s == NULL)
        {
            set_error (_EPTRNULL);
            return -1;
        }
        len += strlen (s);
    }

    return len;
}
