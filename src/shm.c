#include <shm.h>

// todo- temporary value for shmget
#define segsize 100

int shm_flags_def =  SHM_USR_R | SHM_USR_W | SHM_GRP_R | SHM_GRP_W | IPC_CREAT;

int8_t proj_id_def     = 'M';
// todo- make get function
char*  shm_root        = "/tmp/";

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
        err_set (_EALLOC);
        return -1;
    }

    file_touch (shm->path);

    return 0;
}

void shm_generate_key_ftok (shm_t* shm)
{
    shm->key = ftok (shm->path, shm->proj_id);
}
