// todo- make a correct header system, make appropriate values private but find a way to access them, test util functions
#include <test_error.h>
#include <err.h>
int main ()
{
    error_set (_ESUCCESS);
    ERROR_PRINT (0);
    test_error_table ();
    return 0;
}
