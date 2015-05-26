#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <str.h>
#include <err.h>

int test_str         (void);
int test_str_cat     (void);
int test_str_cat_len (void);

#ifndef MM_TEST_STR
#define MM_TEST_STR
extern int has_str_cat;
extern int has_str_cat_len;
#endif
