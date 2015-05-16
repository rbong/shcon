#include <test_file.h>

// todo- find better intermediate return value names

// todo- split up this function
int test_file_touch (void)
{
    int   ret      =  0;
    int   ret_b    =  0;
    FILE* fp       =  NULL;
    char* filename = "/tmp/foobar";
    char  buf [8]  = "";

    err_reset ();

    ret = file_touch (NULL);
    if (ret >= 0)
    {
        ERROR_AT_LINE (0, "file_touch () does not report NULL filenames.");
        if (error_number != _ESUCCESS)
        {
            ERROR_AT_LINE (0, "Note: error_number is set.");
            ERROR_PRINT (0);
            err_reset ();
        }
        ret = -1;
    }

    err_reset ();
    ret = file_touch ("");
    if (ret >= 0)
    {
        ERROR_AT_LINE (0, "file_touch () does not report empty filenames.");
        if (error_number != _ESUCCESS)
        {
            ERROR_AT_LINE (0, "Note: error_number is set.");
            // reset error code
            error_set (_ESUCCESS);
        }
        ret = -1;
    }

    err_reset ();
    ret_b = access (filename, F_OK);
    switch (errno)
    {
    case 0:
        ret_b = unlink (filename);
        if (ret_b < 0)
        {
            ERROR_AT_LINE_SYS (0, errno);
            ERROR_AT_LINE (0, "No access to test file.");
            return ret_b;
        }
        break;
    case ENOENT:
        break;
    default:
        ERROR_AT_LINE_SYS (0, errno);
        ERROR_AT_LINE (0, "access () failed.");
        return -1;
    }
    ret_b = 0;
    err_reset ();

    ret_b = file_touch (filename);

    if (ret_b < 0)
    {
        ERROR_PRINT (0);
        ERROR_AT_LINE (0, "file_touch () failed.");
        return ret_b;
    }

    ret_b = access (filename, F_OK);
    if (ret_b < 0)
    {
        ERROR_AT_LINE_SYS (0, errno);
        ERROR_AT_LINE (0, "file_touch () did not create a file.");
        return ret_b;
    }

    fp = fopen (filename, "w");

    if (fp == NULL)
    {
        ERROR_AT_LINE_SYS (0, errno);
        ERROR_AT_LINE (0, "Could not open file created by file_touch ().");
        return -1;
    }

    ret_b = fprintf (fp, "foobar");

    if (ret_b < 0)
    {
        ERROR_AT_LINE_SYS (0, errno);
        ERROR_AT_LINE (0, "fprintf () failed.");
        return ret_b;
    }

    ret_b = fclose (fp);

    if (ret_b < 0)
    {
        ERROR_AT_LINE_SYS (0, errno);
        ERROR_AT_LINE (0, "fclose () failed.");
        return ret_b;
    }

    ret_b = file_touch (filename);

    if (ret_b < 0)
    {
        ERROR_PRINT (0);
        ERROR_AT_LINE (0, "file_touch () failed.");
        return ret_b;
    }

    ret_b = access (filename, R_OK | W_OK);

    if (ret_b < 0)
    {
        ERROR_AT_LINE_SYS (0, errno);
        ERROR_AT_LINE (0, "file_touch () destroyed file.");
        return ret_b;
    }

    fp = NULL;
    fp = fopen (filename, "r");

    if (fp == NULL)
    {
        ERROR_AT_LINE_SYS (0, errno);
        ERROR_AT_LINE (0, "Could not open file touched by file_touch ().");
        return -1;
    }

    ret_b = fread (buf, 1, 7, fp);

    if (ret_b < 0 && ferror (fp) < 0)
    {
        ERROR_AT_LINE_SYS (0, errno);
        ERROR_AT_LINE (0, "fread () or ferror () failed.");
        fclose (fp);
        return ret_b;
    }

    ret_b = fclose (fp);

    if (ret_b < 0)
    {
        ERROR_AT_LINE_SYS (0, errno);
        ERROR_AT_LINE (0, "fclose () failed.");
    }

    ret_b = strcmp (buf, "foobar");

    if (ret_b != 0)
    {
        ERROR_AT_LINE (0, "file_touch () destroyed file contents.");
        ERROR_AT_LINE (0, "Note: new contents are \"%s\".", buf);
        return -1;
    }

    // todo- test restricted files once behaviour is decided

    if (ret_b < 0)
    {
        ret = -1;
    }
    return ret;
}
