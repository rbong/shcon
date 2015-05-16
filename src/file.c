#include <file.h>

// todo- determine if shm actually needs an untouched file, skip unreadale files and see if shm needs readable files
int file_touch (char* file_name)
{
    int   ret = 0;
    FILE* fp  = NULL;

    if (file_name == NULL)
    {
        error_set (_EPTRNULL);
        return -1;
    }

    if (file_name [0] == '\0')
    {
        // todo- return an error for this
        return -1;
    }

    fp = fopen (file_name, "a+");

    if (fp == NULL)
    {
        ERROR_AT_LINE_SYS (0, errno);
        error_set (_EFOPEN);
        return -1;
    }

    ret = fclose (fp);

    if (ret == EOF)
    {
        ERROR_AT_LINE_SYS (0, errno);
        error_set (_EFCLOSE);
        return -1;
    }

    return 0;
}
