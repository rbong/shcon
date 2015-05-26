#include <stdlib.h>
#include <sys/sem.h>

#include <err.h>
#include <ipc.h>

// todo- comment all guard blocks
/* -- START OF GUARD BLOCK -- */
#ifndef MM_SEM
#define MM_SEM
typedef struct sem_t
{
    ipc_t* ipc;
    int    len;
    int    id;
    int    locked;
} sem_t;

extern int sem_len;

union semun
{
    int              val;
    struct semid_ds* buf;
    unsigned short*  array;
} semun;
#endif
/* --- END OF GUARD BLOCK --- */

int    sem_t_new      (sem_t**);
int    sem_t_set      (sem_t**, ipc_t*, int);
int    sem_t_from_ipc (sem_t**, ipc_t*);
void   sem_t_del      (sem_t*);
int    sem_gen_id     (sem_t*);
int    sem_lock       (sem_t*);
int    sem_unlock     (sem_t*);
