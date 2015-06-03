/** @file shcon_protocol.c
@author Roger Bongers
@date June 2 2015
@brief See shcon_protocol.h.
@see shcon_protocol.h
**/

#include <shcon_protocol.h>

msg_t shcon_msg_init = { MSG_INIT, { MM_HDR_VER, 0, 0 }, NULL };

int shcon_create_sem_id (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    int _flags;

    if (_shcon == NULL || _shcon->ipc == NULL || _shcon->sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    _flags = _shcon->ipc->flags | IPC_CREAT | IPC_EXCL;
    tmp = sem_gen_id (_shcon->sem, _shcon->ipc->key, _flags);
    if (tmp < 0)
    {
        if (errno == EEXIST)
        {
            ret = 1;
        }
        else
        {
            ERR_SYS (errno);
            ERR_PRINT (_ESYSTEM);
            ret = -1;
        }
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
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    _flags = _shcon->ipc->flags & ~IPC_CREAT & ~IPC_EXCL;
    tmp = sem_gen_id (_shcon->sem, _shcon->ipc->key, _flags);
    if (tmp < 0)
    {
        ret = tmp;
        if (errno == ENOENT)
        {
            ret = 1;
        }
        else
        {
            ERR_SYS (errno);
            ERR_PRINT (_ESYSTEM);
            ret = -1;
        }
    }
    return ret;
}

int shcon_init_sem (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;

    if (_shcon == NULL || _shcon->sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    tmp = sem_ctl (_shcon->sem, SEMSET_LOCK, SETVAL, (union sem_un) 0);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }
    _shcon->locked = 1;

    tmp = sem_ctl (_shcon->sem, SEMSET_CON,  SETVAL, (union sem_un) 0);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }

    tmp = sem_ctl (_shcon->sem, SEMSET_READ,  SETVAL, (union sem_un) 0);
    if (tmp < 0)
    {
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
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    _flags = _shcon->ipc->flags | IPC_CREAT | IPC_EXCL;
    tmp = shm_gen_id (_shcon->shm, _shcon->ipc->key, _flags);
    if (tmp < 0)
    {
        if (errno == EEXIST)
        {
            ret = 1;
        }
        else
        {
            ERR_SYS (errno);
            ERR_PRINT (_ESYSTEM);
            ret = -1;
        }
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
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    _flags = _shcon->ipc->flags & ~IPC_CREAT & ~IPC_EXCL;
    tmp = shm_gen_id (_shcon->shm, _shcon->ipc->key, _flags);
    if (tmp < 0)
    {
        if (errno == ENOENT)
        {
            ret = 1;
        }
        else
        {
            ERR_SYS (errno);
            ERR_PRINT (_ESYSTEM);
            ret = -1;
        }
    }
    return ret;
}

int shcon_init_shm (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;

    tmp = shcon_send_shm_msg (_shcon, &shcon_msg_init);
    if (tmp < 0)
    {
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
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    tmp = shcon_send_shm_msg (_shcon, &_msg_kill);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

int shcon_send_shm_msg (shcon_t* _shcon, msg_t* _msg)
{
    int tmp = 0;
    int ret = 0;
    int _offset = 0;
    void* _bmsg = NULL;

    if (_shcon == NULL || _shcon->sem == NULL ||
            _shcon->shm == NULL || _msg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    _bmsg = msg_to_bin (_msg);
    if (_bmsg == NULL)
    {
        ret = -1;
        return ret;
    }

    if (_msg->type != MSG_INIT)
    {
        tmp = shcon_lock_sem (_shcon);
        if (tmp < 0)
        {
            free (_bmsg);
            ret = tmp;
            return ret;
        }
        _offset = sizeof (shcon_msg_init);
    }

    tmp = shm_write (_shcon->shm, _bmsg, sizeof (_bmsg), _offset);
    free (_bmsg);
    if (tmp < 0)
    {
        ret = tmp;
    }
    if (_msg->type != MSG_INIT)
    {
        tmp = shcon_unlock_sem (_shcon);
        if (tmp < 0)
        {
            ret = tmp;
            return ret;
        }
    }
    return ret;
}

int shcon_check_shm_init (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    msg_t* _msg;

    // todo- make check_ver function
    tmp = shcon_lock_sem (_shcon);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }

    _msg = shcon_recv_shm_msg (_shcon, 1);
    tmp = shcon_unlock_sem (_shcon);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }
    if (_msg == NULL)
    {
        ret = -1;
        return ret;
    }

    msg_t_del (&_msg);
    if (_msg->hdr.ver != MM_HDR_VER)
    {
        // todo- new error
        ERR_PRINT (_EBADVAL);
        ret = -1;
    }
    return ret;
}

msg_t* shcon_recv_shm_msg (shcon_t* _shcon, int _init)
{
    int tmp = 0;
    msg_t* ret = msg_t_new ();
    int _offset = 0;

    if (ret == NULL)
    {
        return ret;
    }

    if (_shcon == NULL || _shcon->shm == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = NULL;
        return ret;
    }

    if (!_init)
    {
        tmp = msg_to_bin_len (&shcon_msg_init);
        if (tmp < 0)
        {
            ret = NULL;
            return ret;
        }
        _offset += tmp;
    }
    tmp = shm_read (_shcon->shm, &(ret->type), sizeof (ret->type), _offset);
    if (tmp < 0)
    {
        ret = NULL;
        return ret;
    }
    _offset += sizeof (ret->type);
    tmp = shm_read (_shcon->shm, &(ret->hdr), sizeof (ret->hdr), _offset);
    if (tmp < 0)
    {
        ret = NULL;
        return ret;
    }
    _offset += sizeof (ret->hdr);
    tmp = shm_read (_shcon->shm, ret->data, ret->hdr.len, _offset);
    if (tmp < 0)
    {
        ret = NULL;
    }
    return ret;
}

// todo- make sure you add this to protocol
// todo- define behaviour for marking as read after failures
int shcon_mark_sem (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    struct sembuf _add_buf = { SEMSET_READ, +1, SEM_UNDO };

    if (_shcon == NULL || _shcon->sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    tmp = sem_op (_shcon->sem, _add_buf, 1);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

int shcon_lock_sem (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    struct sembuf _lock_buf = { SEMSET_LOCK, -1, 0 };
    
    if (_shcon == NULL || _shcon->sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if (_shcon->locked)
    {
        ret = 1;
        return ret;
    }

    tmp = sem_op (_shcon->sem, _lock_buf, 1);
    if (tmp < 0)
    {
        ret = tmp;
    }
    else
    {
        _shcon->locked = 1;
    }
    return ret;
}

int shcon_unlock_sem (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    struct sembuf _unlock_buf = { SEMSET_LOCK, +1, 0 };
    
    if (_shcon == NULL || _shcon->sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if (!_shcon->locked)
    {
        ret = 1;
        return ret;
    }

    tmp = sem_op (_shcon->sem, _unlock_buf, 1);
    if (tmp < 0)
    {
        ret = tmp;
    }
    else
    {
        _shcon->locked = 0;
    }
    return ret;
}

int shcon_add_sem_con (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    struct sembuf _add_buf = { SEMSET_CON, +1, SEM_UNDO };

    if (_shcon == NULL || _shcon->sem == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    tmp = sem_op (_shcon->sem, _add_buf, 1);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

// todo- break this up
int shcon_init (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    int _attach;

    if (_shcon == NULL || _shcon->sem == NULL
            || _shcon->shm == NULL || _shcon->ipc == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    // Explicitly create a new sem id
    tmp = _attach = shcon_create_sem_id (_shcon);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }
    // sem id already exists. Attach to new shm and sem
    if (_attach == 1)
    {
        tmp = shcon_attach_sem_id (_shcon);
        if (tmp != 0)
        {
            ret = -1;
            return ret;
        }

        tmp = shcon_attach_shm_id (_shcon);
        if (tmp < 0)
        {
            ret= - 1;
            return ret;
        }
        // sem exists but shm does not. Initiate attachment procedure
        if (tmp == 1)
        {
            _attach = 0;
        }
        else
        {
            // Make sure the shm is the correct version
            tmp = shcon_check_shm_init (_shcon);
            // todo- recover if there are no connections
            if (tmp < 0)
            {
                ret = tmp;
                return ret;
            }
        }
    }
    // sem id did not previously exist and we are now attached
    // Create new shm
    if (_attach == 0)
    {
        tmp = shcon_create_shm_id (_shcon);
        if (tmp < 0)
        {
            ret = tmp;
            return ret;
        }
        // sem is new but shm is not. Attempt to kill connections
        if (tmp == 1)
        {
            tmp = shcon_attach_shm_id (_shcon);
            if (tmp < 0)
            {
                ret = tmp;
                return ret;
            }
            if (tmp > 0)
            {
                // todo- new error
                ERR_SYS (errno);
                ERR_PRINT (_ESYSTEM);
                ret = -1;
                return ret;
            }

            // todo- check for an init message in case this is not a mm shm
            tmp = shcon_kill_shm (_shcon);
            if (tmp < 0)
            {
                ret = tmp;
                return ret;
            }
        }

        // Set values of semaphore
        tmp = shcon_init_sem (_shcon);
        if (tmp < 0)
        {
            ret = tmp;
            return ret;
        }

        tmp = shcon_unlock_sem (_shcon);
        if (tmp < 0)
        {
            ret = tmp;
            return ret;
        }
    }

    tmp = shcon_add_sem_con (_shcon);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}
