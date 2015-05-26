#include <stdlib.h>
#include <string.h>

#include <err.h>

#ifndef MM_MSG
#define MM_MSG
typedef struct msg_hdr_t
{
    int version;
    int date;
    int cmd_len;
    int data_len;
} msg_hdr_t;
// todo- name all structures accordingly instead of having both names the same
typedef struct msg_t
{
    msg_hdr_t hdr;
    char*     cmd;
    char*     data;
} msg_t;
#endif

int msg_get_bin_len (msg_t*);
