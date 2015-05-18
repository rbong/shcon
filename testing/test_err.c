#include <test_err.h>

// todo- split up this function
int test_err_table (void)
{
    int i;
    int ret  = 0;
    int size = ERR_MAX;

    // confirm the table is allocated
    if (size < 1)
    {
        ERR_AT_LINE (0, "err_table size is %d", size);
        ret = -1;
        return ret;
    }

    // confirm indexes match error codes
    for (i = 0; i < size; i++)
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
