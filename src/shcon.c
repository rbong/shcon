#include <shcon.h>

shcon_t* shcon_t_new (void)
{
    shcon_t* _shcon = NULL;

    _shcon = malloc (sizeof (shcon_t));
    if (_shcon == NULL)
    {
        ERR_PRINT (_EALLOC);
        return NULL;
    }

    _shcon->ipc = NULL;
    _shcon->shm = NULL;
    _shcon->sem = NULL;
    return _shcon;
}

int shcon_t_set (shcon_t** _shcon, ipc_t* _ipc, shm_t* _shm, sem_t* _sem)
{
    int tmp = 0;

    if (_shcon == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    if ((*_shcon) == NULL)
    {
        (*_shcon) = shcon_t_new ();
        if ((*_shcon) == NULL)
        {
            ERR_PRINT (_EALLOC);
            return -1;
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

    return 0;
}

int shcon_t_from_path (shcon_t** _shcon, char* _root, char* _sub)
{
    int    tmp  = 0;
    ipc_t* _ipc = NULL;
    shm_t* _shm = NULL;
    sem_t* _sem = NULL;

    tmp = ipc_t_from_path (&_ipc, _root, _sub);
    if (tmp < 0)
    {
        return tmp;
    }

    tmp = shm_t_from_ipc (&_shm, _ipc);
    if (tmp < 0)
    {
        return tmp;
    }

    tmp = sem_t_from_ipc (&_sem, _ipc);
    if (tmp < 0)
    {
        return tmp;
    }

    tmp = shcon_t_set (_shcon, _ipc, _shm, _sem);
    return tmp;
}

void shcon_t_del (shcon_t** _shcon)
{
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

    return;
}

int shcon_send_msg (shcon_t* _shcon, msg_t* _msg)
{
    // todo- make sure len can hold the length of a message
    unsigned long long _len = 0;
    int ret = 0;
    int tmp = 0;
    void* _bin_msg = NULL;

    // todo- clean up NULL checks for values that are passed to functions that check for NULL, write functions for checking all possible NULLs in structures
    if (_shcon == NULL || _msg == NULL || _msg->cmd == NULL || _msg->data == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    tmp = sem_lock (_shcon->sem);
    if (tmp < 0)
    {
        return tmp;
    }

    // todo- write funtion that checks that message _len variables are >= to string lengths, and check before locking earlier in this function
    _bin_msg = msg_get_bin (_msg);
    if (_bin_msg == NULL)
    {
        return -1;
    }

    tmp = shm_write (_shcon->shm, _bin_msg, msg_get_bin_len (_msg));
    if (tmp < 0)
    {
        return tmp;
    }

    // todo- remove extraneous calls to err_reset because of situations like this
    tmp = sem_unlock (_shcon->sem);
    if (tmp < 0)
    {
        return tmp;
    }

    return tmp;
}
