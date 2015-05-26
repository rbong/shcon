#include <shcon.h>

shcon_t* shcon_t_new (void)
{
    /* int tmp = 0; */
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
    /* if (tmp < 0) */
    /* { */
    /*     ret = tmp; */
    /* } */
    return ret;
}

int shcon_t_set (shcon_t** _shcon, ipc_t* _ipc, shm_t* _shm, sem_t* _sem)
{
    /* int tmp = 0; */
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

    /* if (tmp < 0) */
    /* { */
    /*     ret = tmp; */
    /* } */
    return ret;
}

int shcon_t_from_path (shcon_t** _shcon, char* _root, char* _sub)
{
    int    tmp  = 0;
    int    ret  = 0;
    ipc_t* _ipc = NULL;
    shm_t* _shm = NULL;
    sem_t* _sem = NULL;

    tmp = ipc_t_from_path (&_ipc, _root, _sub);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }

    tmp = shm_t_from_ipc (&_shm, _ipc);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }

    tmp = sem_t_from_ipc (&_sem, _ipc);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }

    tmp = shcon_t_set (_shcon, _ipc, _shm, _sem);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

void shcon_t_del (shcon_t** _shcon)
{
    /* int tmp = 0; */
    /* int ret = 0; */

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

    /* if (tmp < 0) */
    /* { */
    /*     ret = tmp; */
    /* } */
    return;
}

int shcon_send_msg (shcon_t* _shcon, msg_t* _msg)
{
    // todo- make sure len can hold the length of a message
    int   tmp      = 0;
    int   ret      = 0;
    void* _bin_msg = NULL;

    // todo- clean up NULL checks for values that are passed to functions that check for NULL, write functions for checking all possible NULLs in structures
    if (_shcon == NULL || _msg == NULL || _msg->cmd == NULL || _msg->data == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    tmp = sem_lock (_shcon->sem);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }

    // todo- write funtion that checks that message _len variables are >= to string lengths, and check before locking earlier in this function
    _bin_msg = msg_to_bin (_msg);
    if (_bin_msg == NULL)
    {
        sem_unlock (_shcon->sem);
        ret = -1;
        return ret;
    }

    tmp = shm_write (_shcon->shm, _bin_msg, msg_to_bin_len (_msg));
    if (tmp < 0)
    {
        // todo- define behaviour if this fails, do more checks
        sem_unlock (_shcon->sem);
        ret = tmp;
        return ret;
    }

    // todo- remove extraneous calls to err_reset because of situations like this
    tmp = sem_unlock (_shcon->sem);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}
