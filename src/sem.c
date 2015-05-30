/** @file sem.c
@author Roger Bongers
@date May 28 2015
@brief See sem.h.
@see sem.h
**/

// todo- decide if _add should be a global variable
#include <sem.h>

int sem_add_conn (sem_t*);

int sem_len = 2;

struct sembuf sem_lock_buf   = { 1, -1, SEM_UNDO };
struct sembuf sem_unlock_buf = { 1, +1, IPC_NOWAIT | SEM_UNDO };

sem_t* sem_t_new (void)
{
    // int tmp = 0;
    sem_t* ret = NULL;

    ret = malloc (sizeof (sem_t));
    if (ret == NULL)
    {
        ERR_PRINT (_EALLOC);
        return NULL;
    }

    ret->len    = 0;
    ret->id     = 0;
    ret->locked = 0;
    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

int sem_t_set (sem_t** _sem, int _id, key_t _key, int _flags)
{
    int tmp = 0;
    int ret = 0;

    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if ((*_sem) == NULL)
    {
        (*_sem) = sem_t_new ();
        if ((*_sem) == NULL)
        {
            ret = -1;
            return ret;
        }
    }

    // not user definable- must have one semaphore for lock, one for members
    (*_sem)->len = sem_len;

    // todo- determine if having no flags is useful
    if (_id == 0 && _key > 0 && _flags >= 0)
    {
        tmp = sem_gen_id ((*_sem), _key, _flags);
        if (tmp < 0)
        {
            ret = tmp;
            return ret;
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

void sem_t_del (sem_t** _sem)
{
    // int tmp = 0;
    // int ret = 0;

    if (_sem == NULL || (*_sem) == NULL)
    {
        return;
    }

    free ((*_sem));
    (*_sem) = NULL;

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return;
}

// todo- determine if this should allocate or not, split this up
int sem_gen_id (sem_t* _sem, key_t _key, int _flags)
{
    int         tmp      = 0;
    int         ret      = 0;
    union semun _sem_ops = { 0 };

    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    // create new semaphore if it doesn't exist
    tmp = _sem->id = semget (_key, _sem->len, _flags);
    if (tmp < 0)
    {
        switch (errno)
        {
            if (errno == EEXIST)
            {
                err_reset ();
            }
            else
            {
                ERR_SYS (errno);
                ERR_PRINT (_ESYSTEM);
                ret = tmp;
                return ret;
            }
        }
    }
    else
    {
        // todo- move this to a function
        // todo- add enums to indicate what semaphore you're on
        _sem_ops.val = 0;
        tmp = semctl (_sem->id, 0, SETVAL, _sem_ops);
        if (tmp < 0)
        {
            ERR_SYS (errno);
            ERR_PRINT (_ESYSTEM);
            ret = tmp;
            return ret;
        }

        _sem_ops.val = 1;
        tmp = semctl (_sem->id, 1, SETVAL, _sem_ops);
        if (tmp < 0)
        {
            ERR_SYS (errno);
            ERR_PRINT (_ESYSTEM);
            ret = tmp;
            return ret;
        }

        tmp = sem_add_conn (_sem);
        if (tmp < 0)
        {
            ret = tmp;
        }
        return ret;
    }

    tmp = _sem->id = semget (_key, _sem->len, _flags);
    if (tmp < 0)
    {
        ERR_SYS (errno);
        // todo- make a new error
        ERR_PRINT (_ESYSTEM);
        ret = tmp;
        return ret;
    }

    tmp = sem_add_conn (_sem);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

// todo- test race conditions, read more on semop
int sem_lock (sem_t* _sem)
{
    // int tmp = 0;
    int ret = 0;

    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if (_sem->locked)
    {
        ret = 1;
        return ret;
    }

    if (semop (_sem->id, &sem_lock_buf, 1) < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
        ret = -1;
        return ret;
    }
    _sem->locked = 1;

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

int sem_unlock (sem_t* _sem)
{
    // int tmp = 0;
    int ret = 0;

    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if (!(_sem->locked))
    {
        ret = 1;
        return ret;
    }

    if (semop (_sem->id, &sem_unlock_buf, 1) < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
        ret = -1;
        return ret;
    }
    _sem->locked = 0;

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

/**
@brief Adds a connection to a sem_t.
@details Called once upon connecting and automatically undone.
Not used external to sem.c.
**/
int sem_add_conn (sem_t* _sem)
{
    int           tmp  = 0;
    int           ret  = 0;
    struct sembuf _add = { 0 };

    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    _add.sem_num = 0;
    _add.sem_op  = 1;
    _add.sem_flg = IPC_NOWAIT | SEM_UNDO;

    tmp = semop (_sem->id, &_add, 1);
    if (tmp < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
        ret = tmp;
    }
    return ret;
}
