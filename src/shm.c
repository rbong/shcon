#include <shm.h>

shm_t* shm_t_new (void)
{
    shm_t* shm = NULL;

    shm = malloc (sizeof (shm_t));

    if (shm == NULL)
    {
        err_set (_EALLOC);
        return NULL;
    }

    shm->ipc  = NULL;
    shm->size = 0;
    shm->id   = 0;
    shm->seg  = NULL;

    return shm;
}

int shm_t_set (shm_t** shm, ipc_t* _ipc, size_t _size, int _id, void* _seg)
{
    int res = 0;
    int ret = 0;

    if (shm == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if ((*shm) == NULL)
    {
        (*shm) = shm_t_new ();

        if ((*shm) == NULL)
        {
            return -1;
        }
    }

    if (_ipc != NULL)
    {
        (*shm)->ipc = _ipc;
    }

    if (_size == 0)
    {
        // todo- get real max
        (*shm)->size = 1028;
    }
    else if (_size > 0)
    {
        (*shm)->size = _size;
    }

    if (_id == 0)
    {
        res = shm_gen_id ((*shm));
        if (res < 0)
        {
            ret = res;
            return ret;
        }
    }
    else if (_id > 0)
    {
        (*shm)->id = _id;
    }

    if (_seg != NULL)
    {
        (*shm)->seg = _seg;
    }

    if (res < 0)
    {
        ret = res;
    }

    return ret;
}

int shm_t_from_path (shm_t** shm, char* root, char* sub)
{
    int res = 0;
    int ret = 0;

    ipc_t* _ipc = NULL;

    res = ipc_t_from_path (&_ipc, root, sub);

    if (res < 0)
    {
        ret = res;
        return ret;
    }

    ret = shm_t_from_ipc (shm, _ipc);
    return ret;
}

int shm_t_from_ipc (shm_t** shm, ipc_t* _ipc)
{
    int res = 0;
    int ret = 0;

    if (_ipc == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    res = shm_t_set (shm, _ipc, 0, 0, NULL);

    if (res < 0)
    {
        ret = res;
        return ret;
    }

    res = shm_attach_seg (*shm);

    if (res < 0)
    {
        ret = res;
    }

    return ret;
}

void shm_t_del (shm_t** shm)
{
    if (shm == NULL || *shm == NULL)
    {
        return;
    }

    if ((*shm)->ipc != NULL)
    {
        ipc_t_del (((*shm)->ipc));
    }

    // todo- maybe something for seg

    free (*shm);
}

int shm_gen_id (shm_t* shm)
{
    int ret    = 0;
    int _flags = 0;
    err_reset ();

    if (shm == NULL || shm->ipc == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    _flags = shm->ipc->flags | IPC_CREAT | IPC_EXCL;
    shm->id = shmget (shm->ipc->key, shm->size, _flags);
    if (shm->id < 0)
    {
        switch (errno)
        {
            case EEXIST:
                err_reset ();
                break;
            default:
                ERR_AT_LINE_SYS (0, errno);
                err_set (_ESYSTEM);
                ret = shm->id;
                return ret;
        }
    }
    else
    {
        // do whatever you need to do on a new shm
        return ret;
    }

    shm->id = shmget (shm->ipc->key, shm->size, shm->ipc->flags);

    if (shm->id < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        // todo- make a new error
        err_set (_ESYSTEM);
        return -1;
    }

    return 0;

    return shm->id;
}

int shm_attach_seg (shm_t* shm)
{
    if (shm == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    shm->seg = shmat (shm->id, 0, 0);

    if (shm->seg < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_ESYSTEM);
        return -1;
    }

    return 0;
}

int shm_write (shm_t* shm, void* buf, int nbytes)
{
    if (shm == NULL || buf == NULL || shm->seg == NULL)
    {
        err_set (_EPTRNULL);
    }

    /* if (strlen (buf) > nbytes)
    {
        // todo- print warning
    } */

    strncpy (shm->seg, buf, nbytes);

    if (shm->seg == NULL)
    {
        return -1;
    }

    return 0;
}

int shm_read (shm_t* shm, void* buf, int noff, int nbytes)
{
    // todo- check that noff does not exceed the maximumm size of the shm
    // todo- urgent- APPEND MESSAGES TO THE SHM
    // todo- urgent- make an init message on all shm, check for it and init semaphore/shm by clearing everything and setting the semaphore if it is not present
    // todo- urgent- make a version indicator on all init messages, have it set by makefile
    // todo- urgent- make msgs void, not char
    if (shm == NULL || buf == NULL || shm->seg == NULL)
    {
        err_set (_EPTRNULL);
    }

    strncpy (buf, shm->seg, nbytes);

    if (buf == NULL)
    {
        return -1;
    }

    return 0;
}
