/** @file shcon_intern.c
@author Roger Bongers
@date June 2 2015
@brief See shcon_intern.h.
@see shcon_intern.h
**/

#include <shcon_intern.h>

int _shcon_create_sem (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    int _flags;

    if (_shcon == NULL || _shcon->ipc == NULL || _shcon->sem == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    _flags = _shcon->ipc->flags | IPC_CREAT | IPC_EXCL;
    tmp = sem_gen_id (_shcon->sem, _shcon->ipc->key, _flags);
    if (tmp < 0)
    {
        ret = -1;
    }
    return ret;
}

int _shcon_attach_sem (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    int _flags;

    if (_shcon == NULL || _shcon->ipc == NULL || _shcon->sem == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    _flags = _shcon->ipc->flags & ~IPC_CREAT & ~IPC_EXCL;
    tmp = sem_gen_id (_shcon->sem, _shcon->ipc->key, _flags);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

int _shcon_init_sem (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;

    if (_shcon == NULL || _shcon->sem == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    tmp = sem_ctl (_shcon->sem, SEMSET_LOCK, SETVAL, (union sem_un) 0);
    if (tmp < 0)
    {
        err_num = _ESUCCESS;
        ret = tmp;
        return ret;
    }
    _shcon->locked = 1;

    tmp = sem_ctl (_shcon->sem, SEMSET_CON,  SETVAL, (union sem_un) 0);
    if (tmp < 0)
    {
        err_num = _ESUCCESS;
        ret = tmp;
        return ret;
    }

    tmp = sem_ctl (_shcon->sem, SEMSET_READ,  SETVAL, (union sem_un) 0);
    if (tmp < 0)
    {
        err_num = _ESUCCESS;
        ret = tmp;
    }
    return ret;
}

int _shcon_create_shm (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    int _flags;

    if (_shcon == NULL || _shcon->ipc == NULL || _shcon->shm == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    _flags = _shcon->ipc->flags | IPC_CREAT | IPC_EXCL;
    tmp = shm_gen_id (_shcon->shm, _shcon->ipc->key, _flags);
    if (tmp < 0)
    {
        ret = tmp;
    }
    else
    {
        tmp = shm_attach_seg (_shcon->shm, NULL, 0);
        if (tmp < 0)
        {
            err_num = _ESUCCESS;
            ret = tmp;
        }
    }
    return ret;
}

int _shcon_attach_shm (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    int _flags;

    if (_shcon == NULL || _shcon->ipc == NULL || _shcon->shm == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    _flags = _shcon->ipc->flags & ~IPC_CREAT & ~IPC_EXCL;
    tmp = shm_gen_id (_shcon->shm, _shcon->ipc->key, _flags);
    if (tmp < 0)
    {
        ret = tmp;
        tmp = shm_attach_seg (_shcon->shm, NULL, 0);
        if (tmp < 0)
        {
            err_num = _ESUCCESS;
            ret = tmp;
        }
    }
    return ret;
}

int _shcon_init_shm (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;

    if (_shcon == NULL || _shcon->shm == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    tmp = shcon_send_shm_msg (_shcon, &shcon_msg_init);
    if (tmp < 0)
    {
        err_num = _ESUCCESS;
        ret = tmp;
    }
    return ret;
}

int _shcon_check_shm_ver (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    msg_t* _msg;

    if (_shcon = NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    _msg = shcon_recv_shm_msg (_shcon, 1);
    if (_msg == NULL)
    {
        err_num = _ESUCCESS;
        ret = -1;
        return ret;
    }

    if (_msg->hdr.ver != MM_HDR_VER)
    {
        // todo- new error
        err_num = _EBADVAL;
        ret = -1;
    }
    msg_t_del (&_msg);
    return ret;
}
