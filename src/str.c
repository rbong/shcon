#include <str.h>
int str_cat (char* dest, char* pre, char* post)
{
    if (size (dest) < str_cat_ln (pre, post))
    {
        set_error (_ESTRSMALL);
        return -1;
    }

    // add better error checking
    strcpy (dest, pre);
    strcpy (dest + strlen (pre), post);

    return 1;
}

int str_cat_ln (char* pre, char* post)
{
    return strlen (pre) + strlen (post) + 1;
}


