#include <sem.h>

int sem_add_conn (sem_t*);

int           sem_len        = 2;
struct sembuf sem_lock_buf   = { 1, -1, SEM_UNDO };
struct sembuf sem_unlock_buf = { 1, +1, IPC_NOWAIT };

sem_t* sem_t_new (void)
{
    sem_t* _sem = NULL;

    _sem = malloc (sizeof (sem_t));
    if (_sem == NULL)
    {
        ERR_PRINT (_EALLOC);
        return NULL;
    }

    _sem->ipc    = NULL;
    _sem->len    = 0;
    _sem->id     = 0;
    _sem->locked = 0;
    return _sem;
}

int sem_t_set (sem_t** _sem, ipc_t* _ipc, int _id)
{
    int tmp = 0;
    int ret = 0;

    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    if ((*_sem) == NULL)
    {
        (*_sem) = sem_t_new ();
        if ((*_sem) == NULL)
        {
            return -1;
        }
    }

    if (_ipc != NULL)
    {
        (*_sem)->ipc = _ipc;
    }

    // not user definable- must have one semaphore for lock, one for members
    (*_sem)->len = sem_len;

    if (_id == 0)
    {
        tmp = sem_gen_id ((*_sem));
        if (tmp < 0)
        {
            return tmp;
        }
    }
    else if (_id > 0)
    {
        (*_sem)->id = _id;
    }


    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

int sem_t_from_ipc (sem_t** _sem, ipc_t* _ipc)
{
    int tmp = 0;
    int ret = 0;

    if (_ipc == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    tmp = sem_t_set (_sem, _ipc, 0);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

void sem_t_del (sem_t** _sem)
{
    if (_sem == NULL || (*_sem) == NULL)
    {
        return;
    }

    if ((*_sem)->ipc != NULL)
    {
        free ((*_sem)->ipc);
    }

    free ((*_sem));
    (*_sem) = NULL;

    return;
}

// todo- determine if this should allocate or not, split this up
int sem_gen_id (sem_t* _sem)
{
    int         ret    = 0;
    int         _flags = 0;
    union semun _sem_ops;

    if (_sem == NULL || _sem->ipc == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    // create new semaphore if it doesn't exist
    _flags = _sem->ipc->flags | IPC_CREAT | IPC_EXCL;
    _sem->id = semget (_sem->ipc->key, _sem->len, _flags);
    if (_sem->id < 0)
    {
        switch (errno)
        {
            case EEXIST:
                err_reset ();
                break;
            default:
                ERR_SYS (errno);
                ERR_PRINT (_ESYSTEM);
                ret = _sem->id;
                return ret;
        }
    }
    else
    {
        // todo- move this to a function
        // todo- add enums to indicate what semaphore you're on
        _sem_ops.val = 0;
        ret = semctl (_sem->id, 0, SETVAL, _sem_ops);
        if (ret < 0)
        {
            ERR_SYS (errno);
            ERR_PRINT (_ESYSTEM);
            return ret;
        }

        _sem_ops.val = 1;
        ret = semctl (_sem->id, 1, SETVAL, _sem_ops);
        if (ret < 0)
        {
            ERR_SYS (errno);
            ERR_PRINT (_ESYSTEM);
            return ret;
        }

        ret = sem_add_conn (_sem);
        if (ret < 0)
        {
            ERR_SYS (errno);
            ERR_PRINT (_ESYSTEM);
            return ret;
        }

        return ret;
    }

    _sem->id = semget (_sem->ipc->key, _sem->len, _sem->ipc->flags);
    if (_sem->id < 0)
    {
        ERR_SYS (errno);
        // todo- make a new error
        ERR_PRINT (_ESYSTEM);
        return -1;
    }

    ret = sem_add_conn (_sem);
    if (ret < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
    }
    return ret;
}

// todo- test race conditions, read more on semop
int sem_lock (sem_t* _sem)
{
    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    if (_sem->locked)
    {
        return 1;
    }

    if (semop (_sem->id, &sem_lock_buf, 1) < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
        return -1;
    }
    _sem->locked = 1;

    return 0;
}

int sem_unlock (sem_t* _sem)
{
    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    if (!(_sem->locked))
    {
        return 1;
    }

    if (semop (_sem->id, &sem_unlock_buf, 1) < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
        return -1;
    }
    _sem->locked = 0;

    return 0;
}

// do not use this external to sem.c
int sem_add_conn (sem_t* _sem)
{
    int           tmp  =  0;
    struct sembuf _add = { 0 };

    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    _add.sem_num = 0;
    _add.sem_op  = 1;
    _add.sem_flg = IPC_NOWAIT | SEM_UNDO;

    tmp = semop (_sem->id, &_add, 1);
    if (tmp < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
    }
    return tmp;
}
