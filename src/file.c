#include <file.h>

// todo- add better error checking
int file_touch (char* file_name)
{
    int ret  = 0;
    FILE* fp = NULL;

    if (file_name == NULL)
    {
        error_set (_EPTRNULL);
        return -1;
    }

    fp = fopen (file_name, "a+");

    if (fp == NULL)
    {
        SYS_ERROR_AT_LINE (0, errno);
        error_set (_EFOPEN);
        return -1;
    }

    ret = fclose (fp);

    if (ret == EOF)
    {
        SYS_ERROR_AT_LINE (0, errno);
        error_set (_EFCLOSE);
        return -1;
    }

    return 0;
}
