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

#ifndef MM_SHM
#define MM_SHM
typedef struct shm_t
{
    ipc_t* ipc;
    size_t size;
    int    id;
    void*  seg;
} shm_t;

#endif

shm_t* shm_t_new       (void);
int    shm_t_set       (shm_t**, ipc_t*, size_t, int, void*);
int    shm_t_from_path (shm_t**, char*, char*);
int    shm_t_from_ipc  (shm_t**, ipc_t*);
void   shm_t_del       (shm_t**);
int    shm_gen_id      (shm_t*);
int    shm_attach_seg  (shm_t*);
int    shm_write       (shm_t*, char*, int);
int    shm_read        (shm_t*, char*, int);
