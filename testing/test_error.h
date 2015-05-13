#include <string.h>
#include <stdio.h>

#include "../src/error.h"

int test_error_table (void);

#ifndef MM_TEST_ERROR
#define MM_TEST_ERROR
#include "test_error.c"
#endif
