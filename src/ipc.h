#include <stdlib.h>
#include <sys/ipc.h>

#include <err.h>
#include <str.h>
#include <file.h>

/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_IPC
#define MM_IPC
enum IPC_FLAGS {
    IPC_USR_R = 0400,
    IPC_USR_W = 0200,
    IPC_GRP_R =  040,
    IPC_GRP_W =  020,
    IPC_OTH_R =   04,
    IPC_OTH_W =   02,
    // defined in in sys/ipc.h
    // IPC_CREAT
    // IPC_EXCL
};

typedef struct
{
    int   flags;
    int   proj_id;
    char* path;
    key_t key;
} ipc_t;

extern key_t (*ipc_gen_key) (char*, int);
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

ipc_t* ipc_t_new        (void);
int    ipc_t_set        (ipc_t**, int, int, char*, key_t);
int    ipc_t_from_path  (ipc_t**, char*, char*);
void   ipc_t_del        (ipc_t**);
char*  ipc_gen_path     (char*, char*);
key_t  ipc_gen_key_ftok (char*, int);
