/** @file shcon.c
@author Roger Bongers
@date May 28 2015
@brief See shcon.h.
@see shcon.h
**/

#include <shcon.h>

shcon_t* shcon_t_new (void)
{
    // int tmp = 0;
    shcon_t* ret = NULL;

    ret = malloc (sizeof (shcon_t));
    if (ret == NULL)
    {
        ERR_PRINT (_EALLOC);
        ret = NULL;
        return ret;
    }

    ret->ipc = NULL;
    ret->shm = NULL;
    ret->sem = NULL;
    ret->locked = 0;
    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

int shcon_t_set (shcon_t** _shcon, ipc_t* _ipc, shm_t* _shm, sem_t* _sem)
{
    // int tmp = 0;
    int ret = 0;

    if (_shcon == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if ((*_shcon) == NULL)
    {
        (*_shcon) = shcon_t_new ();
        if ((*_shcon) == NULL)
        {
            ERR_PRINT (_EALLOC);
            return -1;
            return ret;
        }
    }

    if (_ipc != NULL)
    {
        (*_shcon)->ipc = _ipc;
    }

    if (_shm != NULL)
    {
        (*_shcon)->shm = _shm;
    }

    if (_sem != NULL)
    {
        (*_shcon)->sem = _sem;
    }

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

void shcon_t_del (shcon_t** _shcon)
{
    // int tmp = 0;
    // int ret = 0;

    if (_shcon == NULL || (*_shcon) == NULL)
    {
        return;
    }

    if ((*_shcon)->ipc != NULL)
    {
        ipc_t_del (&((*_shcon)->ipc));
    }

    if ((*_shcon)->shm != NULL)
    {
        shm_t_del (&((*_shcon)->shm));
    }

    if ((*_shcon)->sem != NULL)
    {
        sem_t_del (&((*_shcon)->sem));
    }

    free ((*_shcon));
    (*_shcon) = NULL;

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return;
}
