/** @file sem.c
@author Roger Bongers
@date May 28 2015
@brief See sem.h.
@see sem.h
**/

// todo- decide if _add should be a global variable
#include <sem.h>

//! Default value for sem_t#len.
int sem_len = 3;

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
    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

int sem_t_set (sem_t** _sem, int _len, int _id, key_t _key, int _flags)
{
    int tmp = 0;
    int ret = 0;

    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        err_num = _ENOBLAME;
        ret = -1;
        return ret;
    }

    if ((*_sem) == NULL)
    {
        (*_sem) = sem_t_new ();
        if ((*_sem) == NULL)
        {
            ERR_FROM ();
            err_num = _ENOBLAME;
            ret = -1;
            return ret;
        }
    }

    if (_len == 0)
    {
        (*_sem)->len = sem_len;
    }
    else if (_len > 0)
    {
        (*_sem)->len = _len;
    }

    // todo- determine if having no flags is useful
    if (_id == 0 && _key > 0 && _flags >= 0)
    {
        tmp = sem_gen_id ((*_sem), _key, _flags);
        if (tmp < 0)
        {
            if (errno == EEXIST || errno == ENOENT)
            {
                ret = tmp;
                return ret;
            }
            else
            {
                if (errno != 0)
                {
                    ERR_SYS (errno);
                }
                ERR_PRINT (err_num);
                err_num = _ENOBLAME;
                ret = -1;
            }
        }
    }
    else if (_id > 0)
    {
        (*_sem)->id = _id;
    }

    return ret;
}

int sem_t_from_id (sem_t** _sem, key_t _key, int _flags)
{
    int tmp = 0;
    int ret = 0;

    tmp = sem_t_set (_sem, 0, 0, _key, _flags);
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

int sem_gen_id (sem_t* _sem, key_t _key, int _flags)
{
    int         tmp      = 0;
    int         ret      = 0;

    if (_sem == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    tmp = _sem->id = semget (_key, _sem->len, _flags);
    if (tmp < 0)
    {
        err_num = _ESYSTEM;
        ret = tmp;
    }
    return ret;
}

int sem_ctl (sem_t* _sem, int _sem_num, int _cmd, union sem_un _sem_un)
{
    int tmp = 0;
    int ret = 0;

    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    tmp = semctl (_sem->id, _sem_num, _cmd, _sem_un);
        ret = tmp;
    if (tmp < 0)
    {
        ERR_PRINT (_ESYSTEM);
    }
    return ret;
}

int sem_op (sem_t* _sem, struct sembuf _sem_ops, size_t _num_sem_ops)
{
    int tmp = 0;
    int ret = 0;

    if (_sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    tmp = semop (_sem->id, &_sem_ops, _num_sem_ops);
    if (tmp < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
        ret = tmp;
    }
    return ret;
}
