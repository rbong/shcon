#include <test_file.h>

// todo- find better intermediate return value names

// todo- split up this function, use more constants
int test_file_touch (void)
{
    int   ret       =  0;
    int   ret_b     =  0;
    FILE* fp        =  NULL;
    char* file_name = "/tmp/foobar";
    char  buf [8]   = "";

    err_reset ();

    ret = file_touch (NULL);
    if (ret >= 0)
    {
        ERR_AT_LINE (0, "file_touch () does not report NULL filenames.");
        if (err_number != _ESUCCESS)
        {
            ERR_AT_LINE (0, "Note: err_number is set.");
            ERR_PRINT (0);
        }
        err_reset ();
        ret = -1;
    }

    err_reset ();
    ret = file_touch ("");
    if (ret >= 0)
    {
        ERR_AT_LINE (0, "file_touch () does not report empty filenames.");
        if (err_number != _ESUCCESS)
        {
            ERR_AT_LINE (0, "Note: err_number is set.");
            ERR_PRINT (0);
        }
        err_reset ();
        ret = -1;
    }

    err_reset ();
    ret_b = access (file_name, F_OK);
    // todo- run this through gdb to ensure this is the correct code
    switch (errno)
    {
    case 0:
        ret_b = unlink (file_name);
        if (ret_b < 0)
        {
            ERR_AT_LINE_SYS (0, errno);
            ERR_AT_LINE (0, "No access to test file.");
            return ret_b;
        }
        break;
    case ENOENT:
        err_reset ();
        break;
    default:
        ERR_AT_LINE_SYS (0, errno);
        ERR_AT_LINE (0, "access () failed.");
        return -1;
        break;
    }
    ret_b = 0;
    err_reset ();

    ret_b = file_touch (file_name);

    if (ret_b < 0)
    {
        ERR_PRINT (0);
        ERR_AT_LINE (0, "file_touch () failed.");
        return ret_b;
    }

    ret_b = access (file_name, F_OK);
    if (ret_b < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        ERR_AT_LINE (0, "file_touch () did not create a file.");
        return ret_b;
    }

    fp = fopen (file_name, "w");

    if (fp == NULL)
    {
        ERR_AT_LINE_SYS (0, errno);
        ERR_AT_LINE (0, "Could not open file created by file_touch ().");
        return -1;
    }

    ret_b = fprintf (fp, "foobar");

    if (ret_b < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        ERR_AT_LINE (0, "fprintf () failed.");
        return ret_b;
    }

    ret_b = fclose (fp);

    if (ret_b < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        ERR_AT_LINE (0, "fclose () failed.");
        return ret_b;
    }

    ret_b = file_touch (file_name);

    if (ret_b < 0)
    {
        ERR_PRINT (0);
        ERR_AT_LINE (0, "file_touch () failed.");
        return ret_b;
    }

    ret_b = access (file_name, R_OK | W_OK);

    if (ret_b < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        ERR_AT_LINE (0, "file_touch () destroyed file.");
        return ret_b;
    }

    fp = NULL;
    fp = fopen (file_name, "r");

    if (fp == NULL)
    {
        ERR_AT_LINE_SYS (0, errno);
        ERR_AT_LINE (0, "Could not read file touched by file_touch ().");
        return -1;
    }

    ret_b = fread (buf, 1, 7, fp);

    if (ret_b < 0 && ferror (fp) < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        ERR_AT_LINE (0, "fread () or ferror () failed.");
        fclose (fp);
        return ret_b;
    }

    ret_b = fclose (fp);

    if (ret_b < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        ERR_AT_LINE (0, "fclose () failed.");
    }

    ret_b = strcmp (buf, "foobar");

    if (ret_b != 0)
    {
        ERR_AT_LINE (0, "file_touch () destroyed file contents.");
        ERR_AT_LINE (0, "Note: new contents are \"%s\".", buf);
        return -1;
    }

    // todo- test restricted files once behaviour is decided

    if (ret_b < 0)
    {
        ret = -1;
    }
    return ret;
}
