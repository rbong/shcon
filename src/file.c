#include <file.h>

// todo- determine if shm actually needs an untouched file, skip unreadale files and see if shm needs readable files
int file_touch (char* _file_name)
{
    int   ret = 0;
    FILE* fp  = NULL;

    if (_file_name == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    if (_file_name [0] == '\0')
    {
        ERR_PRINT (_ESTREMPTY);
        return -1;
    }

    ret = file_exists (_file_name);
    if (ret != 0)
    {
        return ret;
    }

    fp = fopen (_file_name, "a+");
    if (fp == NULL)
    {
        ERR_SYS (errno);
        ERR_PRINT (_EFOPEN);
        return -1;
    }

    ret = fclose (fp);
    if (ret == EOF)
    {
        ERR_SYS (errno);
        ERR_PRINT (_EFCLOSE);
        return -1;
    }

    return ret;
}

int file_exists (char* _file_name)
{
    // todo- add ret
    if (_file_name == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    if (_file_name [0] == '\0')
    {
        ERR_PRINT (_ESTREMPTY);
        return -1;
    }

    access (_file_name, F_OK);
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
        ERR_SYS (errno);
        ERR_PRINT (_EACCESS);
        return -1;
    }
}
