#include <stdlib.h>
#include <string.h>

#include <err.h>

#ifndef MM_MSG
#define MM_MSG
typedef struct
{
    int version;
    int date;
    int cmd_len;
    int data_len;
} msg_hdr_t;
// todo- name all structures accordingly instead of having both names the same
typedef struct
{
    msg_hdr_t hdr;
    char*     cmd;
    char*     data;
} msg_t;
#endif

msg_t* msg_t_new      (void);
void   msg_t_del      (msg_t**);
void*  msg_to_bin     (msg_t*);
int    msg_to_bin_len (msg_t*);
