#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <file.h>

int test_file_touch  (void);
int test_file        (void);

#ifndef MM_TEST_FILE
#define MM_TEST_FILE
extern int has_file_touch;
extern int has_file_exists;
#endif
