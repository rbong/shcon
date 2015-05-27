#include <shm.h>

shm_t* shm_t_new (void)
{
    int    tmp = 0;
    shm_t* ret = 0;

    ret = malloc (sizeof (shm_t));
    if (ret == NULL)
    {
        ERR_PRINT (_EALLOC);
        return ret;
    }

    ret->ipc  = NULL;
    ret->size = 0;
    ret->id   = 0;
    ret->seg  = NULL;
    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

int shm_t_set (shm_t** _shm, ipc_t* _ipc, size_t _size, int _id, void* _seg)
{
    int tmp = 0;
    int ret = 0;

    if (_shm == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    if ((*_shm) == NULL)
    {
        (*_shm) = shm_t_new ();
        if ((*_shm) == NULL)
        {
            ret = -1;
            return ret;
        }
    }

    if (_ipc != NULL)
    {
        (*_shm)->ipc = _ipc;
    }

    if (_size == 0)
    {
        // todo- get real max
        (*_shm)->size = 1028;
    }
    else if (_size > 0)
    {
        (*_shm)->size = _size;
    }

    if (_id == 0)
    {
        tmp = shm_gen_id ((*_shm));
        if (tmp < 0)
        {
            ret = tmp;
            return ret;
        }
    }
    else if (_id > 0)
    {
        (*_shm)->id = _id;
    }

    if (_seg != NULL)
    {
        (*_shm)->seg = _seg;
    }

    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

int shm_t_from_ipc (shm_t** _shm, ipc_t* _ipc)
{
    int tmp = 0;
    int ret = 0;

    if (_ipc == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    tmp = shm_t_set (_shm, _ipc, 0, 0, NULL);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }

    tmp = shm_attach_seg (*_shm);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

int shm_t_from_path (shm_t** _shm, char* _root, char* _sub)
{
    int    tmp  = 0;
    int    ret  = 0;
    ipc_t* _ipc = NULL;

    tmp = ipc_t_from_path (&_ipc, _root, _sub);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }

    tmp = shm_t_from_ipc (_shm, _ipc);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

void shm_t_del (shm_t** _shm)
{
    // int tmp = 0;
    // int ret = 0;

    if (_shm == NULL || (*_shm) == NULL)
    {
        return;
    }

    if ((*_shm)->ipc != NULL)
    {
        ipc_t_del (&((*_shm)->ipc));
    }

    // todo- maybe something for seg

    free (*_shm);
    (*_shm) = NULL;

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return;
}

int shm_gen_id (shm_t* _shm)
{
    int tmp    = 0;
    int ret    = 0;
    int _flags = 0;

    if (_shm == NULL || _shm->ipc == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    _flags = _shm->ipc->flags | IPC_CREAT | IPC_EXCL;
    tmp = _shm->id = shmget (_shm->ipc->key, _shm->size, _flags);
    if (tmp < 0)
    {
        if (errno == EEXIST)
        {
            err_reset ();
        }
        else
        {
            ERR_SYS (errno);
            ERR_PRINT (_ESYSTEM);
            ret = _shm->id;
            return ret;
        }
    }
    else
    {
        // todo- do whatever you need to do on a new shm
        return ret;
    }

    tmp = _shm->id = shmget (_shm->ipc->key, _shm->size, _shm->ipc->flags);
    if (tmp < 0)
    {
        ERR_SYS (errno);
        // todo- make a new error
        ERR_PRINT (_ESYSTEM);
        ret = -1;
    }
    return ret;
}

int shm_attach_seg (shm_t* _shm)
{
    // int tmp = 0;
    int ret = 0;

    if (_shm == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    _shm->seg = shmat (_shm->id, 0, 0);
    if (_shm->seg < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
        ret = -1;
    }
    return ret;
}

int shm_read (shm_t* _shm, void* _buf, int _bytes)
{
    // todo- urgent- overhaul this, APPEND MESSAGES TO THE _shm
    // todo- urgent- make an init message on all shm, check for it and init semaphore/shm by clearing everything and setting the semaphore if it is not present
    // todo- urgent- make a version indicator on all init messages, have it set by makefile
    // todo- urgent- make msgs void, not char

    // int tmp = 0;
    int ret = 0;

    if (_shm == NULL || _buf == NULL || _shm->seg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    memcpy (_buf, _shm->seg, _bytes);
    if (_buf == NULL)
    {
        ret = -1;
        return ret;
    }
    return ret;
}

int shm_write (shm_t* _shm, void* _buf, int _bytes)
{
    // int tmp = 0;
    int ret = 0;

    if (_shm == NULL || _buf == NULL || _shm->seg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    memcpy (_shm->seg, _buf, _bytes);
    if (_shm->seg == NULL)
    {
        ret = -1;
    }
    return ret;
}
