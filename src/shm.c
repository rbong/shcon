#include <shm.h>

shm_t* shm_t_new (void)
{
    shm_t* _shm = NULL;

    _shm = malloc (sizeof (shm_t));
    if (_shm == NULL)
    {
        err_set (_EALLOC);
        return NULL;
    }

    _shm->ipc  = NULL;
    _shm->size = 0;
    _shm->id   = 0;
    _shm->seg  = NULL;

    return _shm;
}

int shm_t_set (shm_t** _shm, ipc_t* _ipc, size_t _size, int _id, void* _seg)
{
    int res = 0;
    int ret = 0;

    if (_shm == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if ((*_shm) == NULL)
    {
        (*_shm) = shm_t_new ();

        if ((*_shm) == NULL)
        {
            return -1;
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
        res = shm_gen_id ((*_shm));
        if (res < 0)
        {
            ret = res;
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

    if (res < 0)
    {
        ret = res;
    }

    return ret;
}

int shm_t_from_path (shm_t** _shm, char* _root, char* _sub)
{
    int res = 0;
    int ret = 0;

    ipc_t* _ipc = NULL;

    res = ipc_t_from_path (&_ipc, _root, _sub);

    if (res < 0)
    {
        ret = res;
        return ret;
    }

    ret = shm_t_from_ipc (_shm, _ipc);
    return ret;
}

int shm_t_from_ipc (shm_t** _shm, ipc_t* _ipc)
{
    int res = 0;
    int ret = 0;

    if (_ipc == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    res = shm_t_set (_shm, _ipc, 0, 0, NULL);

    if (res < 0)
    {
        ret = res;
        return ret;
    }

    res = shm_attach_seg (*_shm);

    if (res < 0)
    {
        ret = res;
    }

    return ret;
}

void shm_t_del (shm_t** _shm)
{
    if (_shm == NULL || *_shm == NULL)
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

    return;
}

int shm_gen_id (shm_t* _shm)
{
    int ret    = 0;
    int _flags = 0;
    err_reset ();

    if (_shm == NULL || _shm->ipc == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    _flags = _shm->ipc->flags | IPC_CREAT | IPC_EXCL;
    _shm->id = shmget (_shm->ipc->key, _shm->size, _flags);
    if (_shm->id < 0)
    {
        switch (errno)
        {
            case EEXIST:
                err_reset ();
                break;
            default:
                ERR_AT_LINE_SYS (0, errno);
                err_set (_ESYSTEM);
                ret = _shm->id;
                return ret;
        }
    }
    else
    {
        // todo- do whatever you need to do on a new shm
        return ret;
    }

    _shm->id = shmget (_shm->ipc->key, _shm->size, _shm->ipc->flags);

    if (_shm->id < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        // todo- make a new error
        err_set (_ESYSTEM);
        return -1;
    }

    return 0;

    return _shm->id;
}

int shm_attach_seg (shm_t* _shm)
{
    if (_shm == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    _shm->seg = shmat (_shm->id, 0, 0);

    if (_shm->seg < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_ESYSTEM);
        return -1;
    }

    return 0;
}

int shm_write (shm_t* _shm, void* _buf, int _bytes)
{
    if (_shm == NULL || _buf == NULL || _shm->seg == NULL)
    {
        err_set (_EPTRNULL);
    }

    memcpy (_shm->seg, _buf, _bytes);

    if (_shm->seg == NULL)
    {
        return -1;
    }

    return 0;
}

int shm_read (shm_t* _shm, void* _buf, int _bytes)
{
    // todo- urgent- overhaul this, APPEND MESSAGES TO THE _shm
    // todo- urgent- make an init message on all shm, check for it and init semaphore/shm by clearing everything and setting the semaphore if it is not present
    // todo- urgent- make a version indicator on all init messages, have it set by makefile
    // todo- urgent- make msgs void, not char
    if (_shm == NULL || _buf == NULL || _shm->seg == NULL)
    {
        err_set (_EPTRNULL);
    }

    memcpy (_buf, _shm->seg, _bytes);

    if (_buf == NULL)
    {
        return -1;
    }

    return 0;
}
