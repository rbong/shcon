#include <test_err.h>

int has_err_table = 0;

int test_err (void)
{
    int ret   = 0;
    int ret_b = 0;

    printf ("Testing err_table...\n");
    ret_b = test_err_table ();
    if (ret_b >= 0)
    {
        has_err_table = 1;
        printf ("Testing err_table succeeded.\n");
    }
    else
    {
        ret = -1;
        printf ("Testing err_table failed.\n");
    }

    printf ("\n");

    return ret;
}

int test_err_table (void)
{
    int i;
    int ret  = 0;
    int size = ERR_MAX;

    err_reset ();

    // confirm the table is allocated
    if (size < 1)
    {
        ERR_AT_LINE (0, "err_table size is %d", size);
        ret = -1;
        return ret;
    }

    // confirm indexes match error codes
    for (i = 0; err_table [i].msg [0] != '\0'; i++)
    {
        if (err_table [i].code != i)
        {
            ERR_AT_LINE (0, "Error code mismatch at %d", i);
            ERR_AT_LINE (0, "Note: run scripts/ecodegen.sh and recompile");
            ret = -1;
        }
    }

    // confirm that the maximum value is as expected
    if (i != ERR_MAX)
    {
        ERR_AT_LINE  (0, "err_table has unexpected number of entries");
        ERR_AT_LINE (0, "Note: current value of err_max is %d", i);
        ret  = -1;
    }

    return ret;
}
