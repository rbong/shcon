#include <shcon.h>

shcon_t* shcon_t_new (void)
{
    shcon_t* _shcon = NULL;

    _shcon = malloc (sizeof (shcon_t));

    if (_shcon == NULL)
    {
        err_set (_EALLOC);
        return NULL;
    }

    _shcon->ipc = NULL;
    _shcon->shm = NULL;
    _shcon->sem = NULL;

    return _shcon;
}

int shcon_t_from_path (shcon_t** _shcon, char* _root, char* _sub)
{
    int res = 0;

    if (_shcon == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if (*_shcon == NULL)
    {
        *_shcon = shcon_t_new ();
        if (_shcon == NULL)
        {
            err_set (_EALLOC);
            return -1;
        }
    }

    res = ipc_t_from_path (&((*_shcon)->ipc), _root, _sub);

    if (res < 0)
    {
        return res;
    }

    res = shm_t_from_ipc (&((*_shcon)->shm), (*_shcon)->ipc);

    if (res < 0)
    {
        return res;
    }

    res = sem_t_from_ipc (&((*_shcon)->sem), (*_shcon)->ipc);

    if (res < 0)
    {
        return res;
    }

    return res;
}

void shcon_t_del (shcon_t** _shcon)
{
    if (_shcon == NULL || *_shcon == NULL)
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

    return;
}

int shcon_send_msg (shcon_t* _shcon, msg_t* _msg)
{
    // todo- make sure len can hold the length of a message
    unsigned long long _len = 0;
    int ret = 0;
    int res = 0;
    void* _bin_msg = NULL;

    // todo- clean up NULL checks for values that are passed to functions that check for NULL, write functions for checking all possible NULLs in structures
    if (_shcon == NULL || _msg == NULL || _msg->cmd == NULL || _msg->data == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    res = sem_lock (_shcon->sem);
    if (res < 0)
    {
        return res;
    }

    // todo- write funtion that checks that message _len variables are >= to string lengths, and check before locking earlier in this function
    _bin_msg = msg_get_bin (_msg);
    if (_bin_msg == NULL)
    {
        return -1;
    }

    res = shm_write (_shcon->shm, _bin_msg, msg_get_bin_len (_msg));
    if (res < 0)
    {
        return res;
    }

    // todo- remove extraneous calls to err_reset because of situations like this
    res = sem_unlock (_shcon->sem);
    if (res < 0)
    {
        return res;
    }

    return res;
}
