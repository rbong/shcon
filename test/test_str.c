// todo- test empty strings
#include <test_str.h>

int has_str_cat     = 0;
int has_str_cat_len = 0;

int test_str (void)
{
    int ret   = 0;
    int ret_b = 0;

    printf ("Testing str_cat ()...\n");
    ret_b = test_str_cat ();
    if (ret_b >= 0)
    {
        has_str_cat = 1;
        printf ("Testing str_cat () succeeded.\n");
    }
    else
    {
        ret = -1;
        printf ("Testing str_cat () failed.\n");
    }

    printf ("\n");

    printf ("Testing str_cat_len ()...\n");
    ret_b = test_str_cat_len ();
    if (ret_b >= 0)
    {
        has_str_cat_len = 1;
        printf ("Testing str_cat_len () succeeded.\n");
    }
    else
    {
        ret = -1;
        printf ("Testing str_cat_len () failed.\n");
    }

    printf ("\n");

    return ret;
}

int test_str_cat (void)
{
    int          i         =    1;
    int          ret       =    0;
    int          ret_b     =    0;
    char         dest [13] =   "";
    const char*  src  [4]  = { "foo", "bar", "baz", "qux" };
    const char*  res  [4]  = { "foo", "foobar", "foobarbaz", "foobarbazqux" };

    err_reset ();

    if (str_cat (1, dest, NULL) >= 0 || str_cat (1, NULL, (char**) src) >= 0)
    {
        ERR_AT_LINE (0, "str_cat () does not report NULL pointers.");
        if (err_number != _ESUCCESS)
        {
            ERR_AT_LINE (0, "Note: err_number is set.");
            // reset error code
            err_set (_ESUCCESS);
        }
        ret = -1;
    }
    err_reset ();

    for (i = 1; i <= 4; i++)
    {
        dest [0] = '\0';

        ret_b = str_cat (i, dest, (char**)  src);

        if (ret_b < 0)
        {
            ERR_PRINT (0);
            ERR_AT_LINE (0, "Note: current test case is %d.", i);
            return ret_b;
        }
        if (dest [0] == '\0')
        {
            ERR_AT_LINE (0, "str_cat () did not modify dest.");
            ERR_AT_LINE (0, "Note: current test case is %d.", i);
            return -1;
        }
        if (strcmp (res [i-1], dest) != 0)
        {
            ERR_AT_LINE (0, "str_cat () returns non-matching string.");
            ERR_AT_LINE (0, "Note: current test case is %d.", i);
            return -1;
        }
    }

    if (ret_b < 0)
    {
        ret = -1;
    }
    return ret;
}

int test_str_cat_len (void)
{
    int         i       =    1;
    int         ret     =    0;
    int         ret_b   =    0;
    const char* src [4] = { "foo", "bar", "qaz", "nux" };
    int         res [4] = {  4,     7,     10,    13   };

    err_reset ();

    ret = str_cat_len (1, NULL);
    if (ret >= 0)
    {
        ERR_AT_LINE (0, "str_cat_len () does not report NULL pointers.");
        if (err_number != _ESUCCESS)
        {
            ERR_AT_LINE (0, "Note: err_number is set.");
            // reset error code
            err_set (_ESUCCESS);
        }
        ret = -1;
    }
    else
    {
        ret = 0;
    }
    err_reset ();

    for (i = 1; i <= 4; i++)
    {
        ret_b = str_cat_len (i, (char**) src);
        if (ret_b != res [i-1])
        {
            ERR_AT_LINE (0, "str_cat_len () returns improper length.");
            ERR_AT_LINE (0, "Note: expected %d, got %d.", res [i-1], ret_b);
            ERR_AT_LINE (0, "Note: current test case is %d.", i);
            return -1;
        }
    }

    if (ret_b < 0)
    {
        ret = -1;
    }
    return ret;
}
