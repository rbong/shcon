#include <shm.h>

// todo- temporary value for shmget
#define segsize 100

int  shm_flags_def   =  SHM_USR_R | SHM_USR_W | SHM_GRP_R |
                        SHM_GRP_W | IPC_CREAT;
int  shm_proj_id_def = 'M';
// todo- make get function
char* shm_root    = "/tmp/";

int (*shm_generate_key_func) (shm_t*) = shm_generate_key_ftok;

int shm_t_new (shm_t** shm, char* root, char* subscription, int proj_id,
               int flags, int (*generate_key_func) (shm_t*))
{
    int ret = 0;

    err_reset ();

    if (shm == NULL || root == NULL || subscription == NULL ||
        generate_key_func == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if (*shm == NULL)
    {
        *shm = malloc (sizeof (shm_t));

        if (*shm == NULL)
        {
            ERR_AT_LINE_SYS (0, errno);
            err_set (_EALLOC);
            return -1;
        }
    }

    (*shm)->proj_id =  proj_id;
    (*shm)->id      =  0;
    (*shm)->flags   =  flags;
    (*shm)->key     =  0;
    (*shm)->path    =  NULL;

    ret = shm_assign_path (*shm, root, subscription);

    if (ret < 0)
    {
        return ret;
    }

    ret = generate_key_func (*shm);

    if (ret < 0)
    {
        return ret;
    }
    if ((*shm)->key <= 0)
    {
        err_set (_EBADFUNC);
        return -1;
    }

    ret = shm_generate_id (*shm);

    if (ret < 0)
    {
        return -1;
    }

    return ret;
}

// todo- break me up
int shm_assign_path (shm_t* shm, char* root, char* subscription)
{
    int   ret;
    char* as [2];

    err_reset ();

    if (shm == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if (subscription == NULL || subscription [0] == '\0')
    {
        return 0;
    }

    if (root == NULL || root [0] == '\0')
    {
        root = shm_root;
    }

    as [0] = root;
    as [1] = subscription;
    ret = str_cat_len (2, as);

    if (ret < 0)
    {
        return ret;
    }

    shm->path = malloc (ret);

    if (shm->path == NULL)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_EALLOC);
        return -1;
    }

    ret = str_cat (2, shm->path, as);

    if (ret < 0)
    {
        return ret;
    }

    ret = file_touch (shm->path);

    if (ret < 0)
    {
        return ret;
    }

    return ret;
}

int shm_generate_key_ftok (shm_t* shm)
{
    err_reset ();

    if (shm == NULL || shm->path == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    shm->key = ftok (shm->path, shm->proj_id);

    if (shm->key < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_ESYSTEM);
        return -1;
    }

    return 0;
}

int shm_generate_id (shm_t* shm)
{
    err_reset ();

    if (shm == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    shm->id = shmget (shm->key, segsize, shm->flags);

    return shm->id;
}

int shm_t_del (shm_t** shm)
{
    int ret = 0;

    err_reset ();

    if (shm == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if (*shm == NULL)
    {
        return 0;
    }

    if ((*shm)->path != NULL)
    {
        free ((*shm)->path);
    }

    free (*shm);

    return ret;
}
