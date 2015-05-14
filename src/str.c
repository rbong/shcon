#include <str.h>

int str_cat (int nstr, char* dest, char** src)
{
    int i;
    int len;
    int nbyte = 0;

    if (dest == NULL || src == NULL)
    {
        error_set (_EPTRNULL);
        return -1;
    }
    len = str_cat_len (nstr, src);
    if (len < 0)
    {
        return len;
    }
    if (sizeof (dest) < len)
    {
        error_set (_EPTROVERFLOW);
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
        error_set (_EPTRNULL);
        return -1;
    }

    for (i = 0; i < nstr; i++)
    {
        s = src [i];
        if (s == NULL)
        {
            error_set (_EPTRNULL);
            return -1;
        }
        len += strlen (s);
    }

    return len;
}
