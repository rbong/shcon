#include <ipc.h>
#include <shm.h>
#include <sem.h>
#include <err.h>

/* -- START OF GUARD BLOCK -- */
#ifndef SHCON_H
#define SHCON_H
typedef struct
{
    ipc_t* ipc;
    shm_t* shm;
    sem_t* sem;
} shcon_t;
#endif
/* --- END OF GUARD BLOCK --- */

shcon_t* shcon_t_new       (void);
int      shcon_t_set       (shcon_t**, ipc_t*, shm_t*, sem_t*);
int      shcon_t_from_path (shcon_t**, char*, char*);
void     shcon_t_del       (shcon_t**);
