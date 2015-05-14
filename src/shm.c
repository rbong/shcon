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
    int ret;

    if (root == NULL)
    {
        root = shm_root;
    }

    shm->path = malloc (str_cat_ln (root, subscription));
    ret       = str_cat (shm->path, root, subscription);
    /* check_error ((ret == -1), "shm_assign_path (): str_cat ()", EXIT_ON_ERROR); */

    nc_create_file (shm->path);
}

void shm_generate_key_ftok (shm_t* shm)
{
    shm->key = ftok (shm->path, shm->proj_id);
}
