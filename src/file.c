#include <file.h>

// todo- determine if shm actually needs an untouched file, skip unreadale files and see if shm needs readable files
int file_touch (char* _file_name)
{
    int   tmp = 0;
    int   ret = 0;
    FILE* _fp  = NULL;

    if (_file_name == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if (_file_name [0] == '\0')
    {
        ERR_PRINT (_ESTREMPTY);
        ret = -1;
        return ret;
    }

    tmp = file_exists (_file_name);
    if (tmp != 0)
    {
        ret = tmp;
        return ret;
    }

    _fp = fopen (_file_name, "a+");
    if (_fp == NULL)
    {
        ERR_SYS (errno);
        ERR_PRINT (_EFOPEN);
        ret = -1;
        return ret;
    }

    tmp = fclose (_fp);
    if (tmp == EOF)
    {
        ERR_SYS (errno);
        ERR_PRINT (_EFCLOSE);
        ret = tmp;
    }
    return ret;
}

int file_exists (char* _file_name)
{
    /* int tmp = 0; */
    int ret = 0;

    // todo- add ret
    if (_file_name == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if (_file_name [0] == '\0')
    {
        ERR_PRINT (_ESTREMPTY);
        ret = -1;
        return ret;
    }

    access (_file_name, F_OK);
    switch (errno)
    {
    case 0:
        ret = 1;
        return ret;
        break;
    case ENOENT:
        err_reset ();
        ret = 0;
        return ret;
        break;
    default:
        ERR_SYS (errno);
        ERR_PRINT (_EACCESS);
        ret = -1;
        return ret;
    }
    /* if (tmp < 0) */
    /* { */
    /*     ret = tmp; */
    /* } */
    ret = -1;
    return ret;
}
