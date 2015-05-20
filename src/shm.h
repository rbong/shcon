// todo- sort these out
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>

#include <err.h>
#include <str.h>
#include <file.h>

#ifndef MM_SHM
#define MM_SHM
enum shm_flags_enum {
    SHM_USR_R = 0400,
    SHM_USR_W = 0200,
    SHM_GRP_R =  040,
    SHM_GRP_W =  020,
    SHM_OTH_R =   04,
    SHM_OTH_W =   02,
    SHM_CREAT = IPC_CREAT,
    SHM_EXCL  = IPC_EXCL,
};

typedef struct shm_t {
    char* path;
    char* seg;
    int   flags;
    int   id;
    int   proj_id;
    key_t key;
} shm_t;

int  shm_generate_key_ftok (shm_t*);

extern int (*shm_generate_key_func) (shm_t*);

extern int   shm_flags_def;
extern int   shm_proj_id_def;
extern char* shm_root;
#endif

int   shm_t_new             (shm_t**, char*, char*, int,
                             int, int (*) (shm_t*));
int   shm_generate_key_ftok (shm_t*);
int   shm_assign_path       (shm_t*, char*, char*);
int   shm_generate_id       (shm_t*);
int   shm_attach_seg        (shm_t*);
int   shm_t_del             (shm_t**);
int   shm_write             (shm_t*, char*, int);
int   shm_read              (shm_t*, char*, int);
