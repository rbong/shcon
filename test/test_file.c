#include <test_file.h>

int has_file_touch  = 0;
int has_file_exists = 0;

int test_file_touch_null  (void);
int test_file_touch_empty (void);
int test_file_touch_write (char*, const char*);
int test_file_touch_read  (char*, const char*);

int test_file (void)
{
    int ret   = 0;
    int ret_b = 0;

    // it is difficult to test file exists without similar code
    printf ("Assuming file_exists () succeeds.\n");
    has_file_exists = 1;

    printf ("\n");

    printf ("Testing file_touch ()...\n");
    ret_b = test_file_touch ();
    if (ret_b >= 0)
    {
        has_file_touch = 1;
        printf ("Testing file_touch () succeeded.\n");
    }
    else
    {
        ret = -1;
        printf ("Testing file_touch () failed.\n");
    }

    printf ("\n");

    return ret;
}

// todo- split up this function, use more constants
int test_file_touch (void)
{
    int   ret         =  0;
    int   ret_b       =  0;
    char* file_name   = "/tmp/foobar";
    const char* test_string = "foobar";

    err_reset ();

    ret = test_file_touch_null ();

    ret_b = test_file_touch_empty ();

    if (ret_b < 0)
    {
        ret = ret_b;
    }

    if (has_file_exists)
    {
        ret_b = file_exists (file_name);

        if (ret_b < 0)
        {
            ERR_AT_LINE (0, "file_exists () failed.");
            ERR_PRINT (0);
            return ret_b;
        }
        else if (ret_b > 0)
        {
            ret_b = unlink (file_name);
            if (ret_b < 0)
            {
                ERR_AT_LINE (0, "No access to test file.");
                ERR_AT_LINE_SYS (0, errno);
                return ret_b;
            }
        }
    }
    else
    {
        ERR_AT_LINE (0, "Note: could not test if test file exists.");
    }

    ret_b = test_file_touch_write (file_name, test_string);

    if (ret_b < 0)
    {
        return ret_b;
    }

    ret_b = test_file_touch_read (file_name, test_string);

    if (ret_b < 0)
    {
        return ret_b;
    }

    // todo- test restricted files once behaviour is decided

    if (ret_b < 0)
    {
        ret = -1;
    }
    return ret;
}

int test_file_touch_null (void)
{
    int ret = 0;

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
    else
    {
        ret = 0;
    }

    return ret;
}

int test_file_touch_empty (void)
{
    int ret = 0;

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
    else
    {
        ret = 0;
    }

    return ret;
}

int test_file_touch_write (char* file_name, const char* test_string)
{
    int   ret = 0;
    FILE* fp  = NULL;

    ret = file_touch (file_name);

    if (ret < 0)
    {
        return -1;
    }

    if (ret < 0)
    {
        ERR_AT_LINE (0, "file_touch () failed.");
        ERR_PRINT (0);
        return ret;
    }

    ret = access (file_name, F_OK);
    if (ret < 0)
    {
        ERR_AT_LINE (0, "file_touch () did not create a file.");
        ERR_AT_LINE_SYS (0, errno);
        return ret;
    }

    fp = fopen (file_name, "w");

    if (fp == NULL)
    {
        ERR_AT_LINE (0, "Could not open file created by file_touch ().");
        ERR_AT_LINE_SYS (0, errno);
        return -1;
    }

    ret = fprintf (fp, test_string);

    if (ret < 0)
    {
        ERR_AT_LINE (0, "fprintf () failed.");
        ERR_AT_LINE_SYS (0, errno);
        return ret;
    }

    ret = fclose (fp);

    if (ret < 0)
    {
        ERR_AT_LINE (0, "fclose () failed.");
        ERR_AT_LINE_SYS (0, errno);
        return ret;
    }

    return ret;
}

int test_file_touch_read (char* file_name, const char* test_string)
{
    int   ret     = 0;
    FILE* fp      = 0;
    char  buf [8] = "";

    ret = file_touch (file_name);

    if (ret < 0)
    {
        ERR_AT_LINE (0, "file_touch () failed.");
        ERR_PRINT (0);
        return ret;
    }

    ret = access (file_name, R_OK | W_OK);

    if (ret < 0)
    {
        ERR_AT_LINE (0, "file_touch () destroyed file.");
        ERR_AT_LINE_SYS (0, errno);
        return ret;
    }

    fp = NULL;
    fp = fopen (file_name, "r");

    if (fp == NULL)
    {
        ERR_AT_LINE (0, "Could not read file touched by file_touch ().");
        ERR_AT_LINE_SYS (0, errno);
        return -1;
    }

    ret = fread (buf, 1, 7, fp);

    if (ret < 0 && ferror (fp) < 0)
    {
        ERR_AT_LINE (0, "fread () or ferror () failed.");
        ERR_AT_LINE_SYS (0, errno);
        fclose (fp);
        return ret;
    }

    ret = fclose (fp);

    if (ret < 0)
    {
        ERR_AT_LINE (0, "fclose () failed.");
        ERR_AT_LINE_SYS (0, errno);
    }

    ret = strcmp (buf, test_string);

    if (ret != 0)
    {
        ERR_AT_LINE (0, "file_touch () destroyed file contents.");
        ERR_AT_LINE (0, "Note: new contents are \"%s\".", buf);
        return -1;
    }

    return ret;
}
