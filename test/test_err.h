#include <string.h>
#include <stdio.h>
#include <error.h>

#include <err.h>

int test_err       (void);
int test_err_table (void);

#ifndef MM_TEST_ERR
#define MM_TEST_ERR
extern int has_err_table;
#endif
