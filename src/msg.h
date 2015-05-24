#include <stdlib.h>
#include <string.h>

#include <err.h>

#ifndef MM_MSG
#define MM_MSG
// todo- name all structures accordingly instead of having both names the same
typedef struct msg_t
{
    unsigned long long version;
    unsigned long long date;
    unsigned long long cmd_len;
    unsigned long long data_len;
    char*              cmd;
    char*              data;
} msg_t;
#endif

int msg_get_bin_len (msg_t*);
