// todo- make appropriate values private but find a way to access them, test file functions, make a script to find unused test functions and files, put explicit error printing before unguaranteed error printing, put err_reset () everywhere, capture error output, fail test every function to ensure that they test correctly
#include <stdio.h>

#include <test_err.h>
#include <test_str.h>
#include <test_file.h>
#include <err.h>

// todo- split this up

int main ()
{
    int ret   = 0;
    int ret_b = 0;

    ret_b = test_err ();

    if (ret_b < 0)
    {
        ret = 1;
    }

    ret_b = test_str ();

    if (ret_b < 0)
    {
        ret = 1;
    }

    ret_b = test_file ();

    if (ret_b < 0)
    {
        ret = 1;
    }

    printf ("Done.\n");

    return ret;
}
