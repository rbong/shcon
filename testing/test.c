// todo- make appropriate values private but find a way to access them, test file functions, make a script to find unused test functions and files, put explicit error printing before unguaranteed error printing, put err_reset () everywhere, capture error output, fail test every function to ensure that they test correctly
#include <test_err.h>
#include <test_str.h>
#include <test_file.h>
#include <err.h>

int main ()
{
    // reset error code
    err_reset ();
    test_err_table ();
    err_reset ();
    test_str_cat ();
    err_reset ();
    test_str_cat_len ();
    err_reset ();
    test_file_touch ();
    return 0;
}
