#include <shcon.h>

// todo- set all local variable names to have underscores in ipc.c and sem.c
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

int shcon_t_from_path (shcon_t** _shcon, char* root, char* sub)
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

    res = ipc_t_from_path (&((*_shcon)->ipc), root, sub);

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
        ipc_t_del ((*_shcon)->ipc);
    }

    if ((*_shcon)->shm != NULL)
    {
        shm_t_del (&((*_shcon)->shm));
    }

    if ((*_shcon)->sem != NULL)
    {
        sem_t_del (((*_shcon)->sem));
    }

    return;
}

int shcon_send_msg (shcon_t* shcon, msg_t* msg)
{
    // todo- make sure len can hold the length of a message
    unsigned long long len = 0;
    int ret = 0;
    int res = 0;

    // todo- clean up NULL checks for values that are passed to functions that check for NULL, write functions for checking all possible NULLs in structures
    if (shcon == NULL || msg == NULL || msg->cmd == NULL || msg->data == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    res = sem_lock (shcon->sem);
    if (res < 0)
    {
        return res;
    }

    // todo- write funtion that checks that message _len variables are >= to string lengths, and check before locking earlier in this function
    // todo- split this up
    res = shm_write (shcon->shm, &(msg->date), sizeof (msg->date));
    len += sizeof (msg->data);
    res = shm_write (shcon->shm, &(msg->cmd_len), sizeof (msg->cmd_len));
    len += sizeof (msg->cmd_len);
    res = shm_write (shcon->shm, &(msg->data_len), sizeof (msg->data_len));
    len += sizeof (msg->data_len);
    res = shm_write (shcon->shm, msg->cmd, msg->cmd_len);
    len += msg->cmd_len;
    res = shm_write (shcon->shm, msg->data, msg->data_len);
    len += msg->data_len;

    // todo- remove extraneous calls to err_reset because of situations like this
    res = sem_unlock (shcon->sem);
    if (res < 0)
    {
        return res;
    }

    return res;
}
