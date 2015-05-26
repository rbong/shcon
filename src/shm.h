// malloc (), free ()
#include <stdlib.h>
// IPC_CREAT, IPC_EXCL, ftok ()
#include <sys/ipc.h>
// shmget (), shmat ()
#include <sys/shm.h>

#include <ipc.h>
#include <err.h>
#include <str.h>
#include <file.h>
#include <msg.h>

/* -- START OF GUARD BLOCK -- */
#ifndef MM_SHM
#define MM_SHM
typedef struct
{
    ipc_t* ipc;
    size_t size;
    int    id;
    void*  seg;
} shm_t;
#endif
/* --- END OF GUARD BLOCK --- */

shm_t* shm_t_new       (void);
int    shm_t_set       (shm_t**, ipc_t*, size_t, int, void*);
int    shm_t_from_ipc  (shm_t**, ipc_t*);
int    shm_t_from_path (shm_t**, char*, char*);
void   shm_t_del       (shm_t**);
int    shm_gen_id      (shm_t*);
int    shm_attach_seg  (shm_t*);
int    shm_read        (shm_t*, void*, int);
int    shm_write       (shm_t*, void*, int);
