#include <test_str.h>

int test_str_cat (void)
{
    char *dest       = NULL;
    const char **src = "test ", "str ", "cat";
    const char **nul = "test ", NULL,   "cat";
    const char *out  = "test str cat";

    dest = malloc (strlen (out) + 1);
    if (str_cat (3, dest, src) < 0)
    {
    }
}

int test_str_cat_len (void)
{
}
