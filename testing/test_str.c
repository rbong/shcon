#include <test_str.h>

// todo- move the string production to a str function
int test_str_cat (int n)
{
    int    i  = 0;
    char*  s  = NULL;
    char** as = NULL;

    as = malloc ((n + 1) * sizeof (char*));
    
    if (as == NULL)
    {
        ERROR_AT_LINE_SYS (0, errno);
        return -1;
    }

    for (i = 0; i < n; i++)
    {
        as [i] = malloc (2);
        if (as [i] == NULL)
        {
            ERROR_AT_LINE_SYS (0, errno);
            return -1;
        }
        strcpy (as [i], " ");
    }

    s = malloc (n + 1);

    if (s == NULL)
    {
        ERROR_AT_LINE_SYS (0, errno);
        return -1;
    }

    

    return 0;
}

int test_str_cat_len (void)
{
    return 0;
}
