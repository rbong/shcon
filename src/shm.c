// sort these out
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>

#include <shm.h>
#include <err.h>
#include <str.h>

// temporary value for shmget
#define segsize 100

// move me to  the main file
int main (int argc, char** argv)
{
    shm_t shm;

    shm_t_new (&shm);

    shm_assign_path (&shm, NULL, "mysub");

    // delete me
    printf ("%s\n", shm.path);

    shm_generate_key_func (&shm);
/*    check_error ((shm.key == -1), "main (): shm_generate_key_func ()"
                 EXIT_ERROR | USE_ERRNO);*/

    // segsize is temporary value
    // shm_id = shmget (shm_key, segsize, 
}

void shm_t_new (shm_t* shm)
{
    shm->proj_id =  proj_id_def;
    shm->id      =  0;
    shm->flags   =  shm_flags_def;
    shm->key     =  0;
    shm->path    =  NULL;
}

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
