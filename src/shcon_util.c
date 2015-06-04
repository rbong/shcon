/** @file shcon_util.c
@author Roger Bongers
@date June 2 2015
@brief See shcon_util.h.
@see shcon_util.h
**/

#include <shcon_util.h>

int shcon_create_sem_id (shcon_t* _shcon)
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

int shcon_attach_sem_id (shcon_t* _shcon)
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

int shcon_init_sem (shcon_t* _shcon)
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

int shcon_create_shm_id (shcon_t* _shcon)
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
    return ret;
}

int shcon_attach_shm_id (shcon_t* _shcon)
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
    }
    return ret;
}

int shcon_init_shm (shcon_t* _shcon)
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

int shcon_kill_shm (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    msg_t _msg_kill = { MSG_KILL, { MM_HDR_VER, 0, 0 }, NULL };

    if (_shcon == NULL || _shcon->shm == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    tmp = shcon_send_shm_msg (_shcon, &_msg_kill);
    if (tmp < 0)
    {
        err_num = _ESUCCESS;
        ret = tmp;
    }
    return ret;
}

int shcon_check_shm_ver (shcon_t* _shcon)
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

    // todo- make check_ver function
    tmp = shcon_lock_sem (_shcon);
    if (tmp < 0)
    {
        err_num = _ESUCCESS;
        ret = tmp;
        return ret;
    }

    _msg = shcon_recv_shm_msg (_shcon, 1);
    tmp = shcon_unlock_sem (_shcon);
    if (_msg == NULL)
    {
        err_num = _ESUCCESS;
        ret = -1;
        return ret;
    }
    if (tmp < 0)
    {
        err_num = _ESUCCESS;
        ret = tmp;
        msg_t_del (&_msg);
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

int shcon_create_sem_shm (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;

    if (_shcon == NULL || _shcon->sem == NULL || _shcon->shm == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    if (_shcon->sem->id == 0)
    {
        tmp = shcon_create_sem_id (_shcon);
        if (tmp < 0)
        {
            ret = tmp;
            return ret;
        }
    }

    tmp = shcon_create_kill_shm (_shcon);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }

    tmp = shcon_init_sem (_shcon);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }

    tmp = shcon_unlock_sem (_shcon);
    if (tmp < 0)
    {
        err_num = _ESUCCESS;
        ret = tmp;
    }
    return ret;
}

// todo- make this wait for the timeout before erasing the kill message
// todo- make this actually delete the shm
int shcon_create_kill_shm (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;

    if (_shcon == NULL || _shcon->shm == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    tmp = shcon_create_shm_id (_shcon);
    if (tmp < 0 && errno != EEXIST)
    {
        ret = tmp;
        return ret;
    }
    if (tmp < 0)
    {
        err_num = 0;
        tmp = shcon_attach_shm_id (_shcon);
        if (tmp < 0)
        {
            ret = tmp;
            return ret;
        }

        tmp = shcon_kill_shm (_shcon);
        if (tmp < 0)
        {
            ret = tmp;
            return ret;
        }
    }
    else
    {
        tmp = shcon_init_shm (_shcon);
        if (tmp < 0)
        {
            ret = tmp;
            return ret;
        }
    }
    return ret;
}

int shcon_attach_sem_shm (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;

    if (_shcon == NULL || _shcon->sem == NULL || _shcon->shm == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    if (_shcon->sem->id == 0)
    {
        tmp = shcon_attach_sem_id (_shcon);
        if (tmp < 0)
        {
            ret = -1;
            return ret;
        }
    }

    tmp = shcon_attach_shm_id (_shcon);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }
    // Make sure the shm is the correct version
    tmp = shcon_check_shm_ver (_shcon);
    // todo- recover if there are no connections
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}
