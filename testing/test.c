// todo- make appropriate values private but find a way to access them, test file functions, make a script to find unused test functions and files, put explicit error printing before unguaranteed error printing, put error_reset () everywhere, capture error output, fail test every function to ensure that they test correctly
#include <test.h>
int main ()
{
    // reset error code
    err_reset ();
    test_error_table ();
    err_reset ();
    test_str_cat ();
    err_reset ();
    test_str_cat_len ();
    err_reset ();
    test_file_touch ();
    return 0;
}
