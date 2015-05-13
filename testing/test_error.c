// todo- split up this function
int test_error_table (void)
{
    int i;
    int ret  = 0;
    int size = sizeof (error_table) / sizeof (error_info_t) - 1;

    // confirm the table is allocated
    if (size < 1)
    {
        ERROR_AT_LINE (0, 0, "error_table size is %d", size);
        ret = -1;
        return ret;
    }

    // confirm indexes match error codes
    for (i = 0; i < size; i++)
    {
        if (error_table [i].code != i)
        {
            ERROR_AT_LINE (0, 0, "Error code mismatch at %d", i);
            ERROR_AT_LINE (0, 0, "Note: run scripts/ecodegen.sh and recompile");
            ret = -1;
        }
    }

    // confirm that the maximum value is as expected
    if (i != ERROR_MAX)
    {
        ERROR_AT_LINE  (0, 0, "error_table has unexpected number of entries");
        ERROR_AT_LINE (0, 0, "Note: current value of error_max is %d", i);
        ret  = -1;
    }
    return ret;
}
