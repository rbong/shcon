// todo- sort these out
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>

#include <shm.h>
#include <err.h>
#include <str.h>

// todo- temporary value for shmget
#define segsize 100

// todo- allocate pointers, create complimentary freeing function
void shm_t_new (shm_t* shm)
{
    shm->proj_id =  proj_id_def;
    shm->id      =  0;
    shm->flags   =  shm_flags_def;
    shm->key     =  0;
    shm->path    =  NULL;
}

// todo- break me up
int shm_assign_path (shm_t* shm, char* root, char* subscription)
{
    int   ret;
    char* as [2];

    if (shm == NULL || shm->path == NULL || subscription == NULL)
    {
        set_error (_EPTRNULL);
        return -1;
    }

    if (root == NULL)
    {
        root = shm_root;
    }
    as [0] = root;
    as [1] = subscription;

    ret = str_cat_ln (2, as);
    if (ret < 0)
    {
        return ret;
    }
    // todo- move this maybe, shift blame
    shm->path = malloc (str_cat_ln (2, as));
    if (shm->path == NULL)
    {
        SYS_ERROR_AT_LINE (0, errno);
        error_set (_EALLOC);
        return -1;
    }

    // todo- fill in file function
    nc_create_file (shm->path);
}

void shm_generate_key_ftok (shm_t* shm)
{
    shm->key = ftok (shm->path, shm->proj_id);
}
