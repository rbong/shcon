#include <file.h>

// todo- determine if shm actually needs an untouched file, skip unreadale files and see if shm needs readable files
int file_touch (char* file_name)
{
    int   ret = 0;
    FILE* fp  = NULL;

    if (file_name == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if (file_name [0] == '\0')
    {
        err_set (_ESTREMPTY);
        return -1;
    }

    ret = file_exists (file_name);

    if (ret != 0)
    {
        return ret;
    }

    fp = fopen (file_name, "a+");

    if (fp == NULL)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_EFOPEN);
        return -1;
    }

    ret = fclose (fp);

    if (ret == EOF)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_EFCLOSE);
        return -1;
    }

    return ret;
}

// todo- test
int file_exists (char* file_name)
{
    err_reset ();

    if (file_name == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if (file_name [0] == '\0')
    {
        err_set (_ESTREMPTY);
        return -1;
    }

    access (file_name, F_OK);
    switch (errno)
    {
    case 0:
        return 1;
        break;
    case ENOENT:
        err_reset ();
        return 0;
        break;
    default:
        ERR_AT_LINE_SYS (0, errno);
        err_set (_EACCESS);
        return -1;
    }
}
