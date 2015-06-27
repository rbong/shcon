/** @file shcon.c
@author Roger Bongers
@date May 28 2015
@brief See shcon.h.
@see shcon.h
**/

#include <shcon.h>
#include <shcon_intern.h>

msg_t shcon_msg_init = { MSG_INIT, { MM_HDR_VER, 0, 0, 1 }, "" };

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
    ret->prev_time = 0;
    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

int shcon_t_set (shcon_t** _shcon, ipc_t* _ipc, shm_t* _shm, sem_t* _sem)
{
    int tmp = 0;
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
            ERR_FROM ();
            return -1;
            return ret;
        }
    }

    if (_ipc != NULL)
    {
        (*_shcon)->ipc = _ipc;
    }
    else
    {
        tmp = ipc_t_set (&((*_shcon)->ipc), 0, NULL, 0);
        if (tmp < 0)
        {
            ERR_FROM ();
            ret= tmp;
            return ret;
        }
    }

    if (_shm != NULL)
    {
        (*_shcon)->shm = _shm;
    }
    else
    {
        tmp = shm_t_set (&((*_shcon)->shm), 0, 0, NULL, 0, 0);
        if (tmp < 0)
        {
            ERR_FROM ();
            ret = tmp;
            return ret;
        }
    }

    if (_sem != NULL)
    {
        (*_shcon)->sem = _sem;
    }
    else
    {
        tmp = sem_t_set (&((*_shcon)->sem), 0, 0, 0, 0);
        if (tmp < 0)
        {
            ERR_FROM ();
            ret = tmp;
            return ret;
        }
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

int shcon_send_empty_shm_msg (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    static msg_t _shcon_msg_empty =
        { MSG_EMPT, { MM_HDR_VER, 0, 0, 1 }, "" };

    tmp = shcon_send_shm_msg (_shcon, &_shcon_msg_empty);
    if (tmp < 0)
    {
        ERR_FROM ();
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

    if (_shcon == NULL || _shcon->shm == NULL || _msg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if (!(_shcon->locked))
    {
        // todo- new error
        ERR_PRINT (_EBADVAL);
        ret = -1;
        return ret;
    }

    _bmsg = msg_to_raw (_msg);
    if (_bmsg == NULL)
    {
        ERR_FROM ();
        ret = -1;
        return ret;
    }

    if (_msg->type != MSG_INIT)
    {
        _offset = msg_to_raw_len (&shcon_msg_init);
    }

    tmp = shm_write (_shcon->shm, _bmsg, msg_to_raw_len (_msg), _offset);
    free (_bmsg);
    if (tmp < 0)
    {
        ret = tmp;
        return ret;
    }

    tmp = _shcon_set_prev_time (_shcon, _msg);
    if (tmp < 0)
    {
        ERR_FROM ();
        ret = tmp;
    }
    return ret;
}

int shcon_recv_shm_msg (shcon_t* _shcon, msg_t** _msg, int _init)
{
    int tmp = 0;
    int ret = 0;
    int _offset = 0;

    if (_shcon == NULL || _shcon->shm == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if (!(_shcon->locked))
    {
        // todo- new error
        ERR_PRINT (_EBADVAL);
        ret = -1;
        return ret;
    }

    (*_msg) = msg_t_new ();
    if (_msg == NULL || (*_msg) == NULL)
    {
        ERR_FROM ();
        ret = -1;
        return ret;
    }

    if (!_init)
    {
        tmp = msg_to_raw_len (&shcon_msg_init);
        if (tmp < 0)
        {
            ERR_FROM ();
            msg_t_del (_msg);
            ret = -1;
            return ret;
        }
        _offset += tmp;
    }
    tmp = shm_read
            (_shcon->shm, &((*_msg)->type), sizeof (enum _MSG_TYPE), _offset);
    if (tmp < 0)
    {
        ERR_FROM ();
        msg_t_del (_msg);
        ret = -1;
        return ret;
    }
    _offset += sizeof ((*_msg)->type);
    tmp = shm_read
            (_shcon->shm, &((*_msg)->hdr), sizeof (msg_hdr_t), _offset);
    if (tmp < 0)
    {
        ERR_FROM ();
        msg_t_del (_msg);
        ret = -1;
        return ret;
    }
    if ((*_msg)->hdr.len != 0)
    {
        (*_msg)->data = malloc ((*_msg)->hdr.len);
        if ((*_msg)->data == NULL)
        {
            ERR_PRINT (_EALLOC);
            ret = -1;
            return ret;
        }

        _offset += sizeof ((*_msg)->hdr);
        tmp = shm_read
            (_shcon->shm, (*_msg)->data, (*_msg)->hdr.len, _offset);
        if (tmp < 0)
        {
            ERR_FROM ();
            msg_t_del (_msg);
            ret = -1;
            return ret;
        }
    }

    tmp = _shcon_check_prev_time (_shcon, (*_msg));
    ret = tmp;
    if (tmp < 0)
    {
        ERR_FROM ();
        return ret;
    }

    if (tmp)
    {
        tmp = _shcon_set_prev_time (_shcon, (*_msg));
        if (tmp < 0)
        {
            ERR_FROM ();
            ret = tmp;
        }
    }
    return ret;
}

int shcon_reset_sem_read (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;

    tmp = sem_ctl (_shcon->sem, SEMSET_READ,  SETVAL, (union sem_un) 0);
    if (tmp < 0)
    {
        err_num = _ENOBLAME;
        ERR_FROM ();
        ret = tmp;
    }
    return ret;
}

int shcon_mark_sem_read (shcon_t* _shcon)
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
        ERR_FROM ();
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
        ERR_FROM ();
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
        ERR_FROM ();
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
        ERR_FROM ();
        ret = tmp;
    }
    return ret;
}

int shcon_get_sem_con (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;

    tmp = sem_ctl (_shcon->sem, SEMSET_CON, GETVAL, (union sem_un) 0);
    ret = tmp;
    if (tmp < 0)
    {
        ERR_PRINT (_ESYSTEM);
    }
    return ret;
}

int shcon_get_sem_read (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;

    tmp = sem_ctl (_shcon->sem, SEMSET_READ, GETVAL, (union sem_un) 0);
    ret = tmp;
    if (tmp < 0)
    {
        ERR_PRINT (_ESYSTEM);
    }
    return ret;
}

int shcon_connect (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    int _init_shm = 0;

    if (_shcon == NULL || _shcon->shm == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }
    // we need to check shm id later to see if it is created
    if (_shcon->shm->id != 0)
    {
        ERR_PRINT (_EBADVAL);
        ret = -1;
        return ret;
    }

    errno = 0;
    tmp =  _shcon_create_sem (_shcon);
    if (tmp < 0 && errno != EEXIST)
    {
        if (err_num != _ENOBLAME)
        {
            if (errno != 0)
            {
                ERR_SYS (errno);
            }
            ERR_PRINT (err_num);
        }
        else
        {
            ERR_FROM ();
        }
        ret = tmp;
        return ret;
    }
    // if the semaphore exists, attach to semaphore and shared memory
    if (tmp < 0)
    {
        // return 0 if the shared memory is attached
        ret = 0;

        errno = 0;
        tmp = _shcon_attach_sem (_shcon);
        // we have gotten a failure trying to connect and attach to semaphore
        if (tmp < 0)
        {
            if (err_num != _ENOBLAME)
            {
                if (errno != 0)
                {
                    ERR_SYS (errno);
                }
                ERR_PRINT (err_num);
            }
            else
            {
                ERR_FROM ();
            }
            ret = tmp;
            return ret;
        }
        errno = 0;
        tmp = _shcon_attach_shm (_shcon);
        // semaphore exists and shared memory does not
        if (tmp < 0 && errno != ENOENT)
        {
            if (err_num != _ENOBLAME)
            {
                if (errno != 0)
                {
                    ERR_SYS (errno);
                }
                ERR_PRINT (err_num);
            }
            else
            {
                ERR_FROM ();
            }
            ret = tmp;
            return ret;
        }
        // attachment to sem and shm was successful, check header version
        else if (tmp >= 0)
        {
            tmp = shcon_lock_sem (_shcon);
            if (tmp < 0)
            {
                ERR_FROM ();
                ret = tmp;
                return ret;
            }

            tmp = _shcon_check_shm_ver (_shcon);
            if (tmp < 0)
            {
                if (err_num != _EBADVAL)
                {
                    ERR_FROM ();
                    shcon_unlock_sem (_shcon);
                    ret = tmp;
                    return ret;
                }
                else
                {
                    tmp = shcon_get_sem_con (_shcon);
                    if (tmp > 0)
                    {
                        // todo- new error
                        ERR_PRINT (_EBADVAL);
                    }
                    if (tmp < 0)
                    {
                        ERR_FROM ();
                    }
                    if (tmp != 0)
                    {
                        shcon_unlock_sem (_shcon);
                        ret = tmp;
                        return ret;
                    }
                    // no connections. recreate semaphore
                    _init_shm = 1;
                }
            }
        }
    }

    // shared memory or semaphore is new
    if (_shcon->shm->id == 0)
    {
        // return 1 if the shared memory is new
        ret = 1;

        tmp = _shcon_init_sem (_shcon);
        if (tmp < 0)
        {
            ERR_FROM ();
            shcon_unlock_sem (_shcon);
            ret = tmp;
            return ret;
        }

        errno = 0;
        tmp = _shcon_create_shm (_shcon);
        if (tmp < 0 && errno != EEXIST)
        {
            if (err_num != _ENOBLAME)
            {
                if (errno != 0)
                {
                    ERR_SYS (errno);
                }
                ERR_PRINT (err_num);
            }
            else
            {
                ERR_FROM ();
            }
            shcon_unlock_sem (_shcon);
            ret = tmp;
            return ret;
        }
        // semaphore is new and shared memory is not. attach
        if (tmp < 0)
        {
            errno = 0;
            tmp = _shcon_attach_shm (_shcon);
            if (tmp < 0)
            {
                if (err_num != _ENOBLAME)
                {
                    if (errno != 0)
                    {
                        ERR_SYS (errno);
                    }
                    ERR_PRINT (err_num);
                }
                else
                {
                    ERR_FROM ();
                }
                shcon_unlock_sem (_shcon);
                ret = tmp;
                return ret;
            }
        }

        _init_shm = 1;
    }

    if (_init_shm)
    {
        tmp = _shcon_init_shm (_shcon);
        if (tmp < 0)
        {
            ERR_FROM ();
            shcon_unlock_sem (_shcon);
            ret = tmp;
            return ret;
        }
    }

    // mark the connection in the smaphore
    tmp = shcon_add_sem_con (_shcon);
    if (tmp < 0)
    {
        ERR_FROM ();
        shcon_unlock_sem (_shcon);
        ret = tmp;
    }
    return ret;
}

int shcon_msg_loop
  (shcon_t* _shcon, int _create, void* _con, int (*_f) (void*, msg_t**, int))
{
    int tmp = 0;
    int ret = 0;
    int _unread = 0;
    int _timeout = 0;
    msg_t* _msg = NULL;

    if (_shcon == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    while (1)
    {
        _unread = 0;
        // forces writing the message if we are not attaching
        _timeout = 1;
        if (!_create)
        {
            tmp = shcon_unlock_sem (_shcon);
            if (tmp < 0)
            {
                ERR_FROM ();
                ret = tmp;
                return ret;
            }

            tmp = _shcon_wait (0);
            if (tmp < 0)
            {
                ERR_FROM ();
                ret = tmp;
                return ret;
            }

            tmp = shcon_lock_sem (_shcon);
            if (tmp < 0)
            {
                ERR_FROM ();
                ret = tmp;
                return ret;
            }

            tmp = shcon_recv_shm_msg (_shcon, &_msg, 0);
            if (tmp < 0)
            {
                ERR_FROM ();
                ret = tmp;
                return ret;
            }

            // message is new
            if (tmp > 0)
            {
                tmp = shcon_mark_sem_read (_shcon);
                if (tmp < 0)
                {
                    ERR_FROM ();
                    ret = tmp;
                    return ret;
                }

                tmp = _f (_con, &_msg, ANON_SEND);
                if (tmp < 0)
                {
                    ERR_FROM ();
                    ret = tmp;
                    return ret;
                }

                if (_msg->type == MSG_KILL)
                {
                    ret = 0;
                    return ret;
                }
            }

            tmp = _timeout = _shcon_timed_out_msg (_msg);
            if (tmp < 0)
            {
                ERR_FROM ();
                ret = tmp;
                return ret;
            }

            if (!_timeout)
            {
                tmp = _unread = _shcon_has_unread_sem (_shcon);
                if (tmp < 0)
                {
                    ERR_FROM ();
                    ret = tmp;
                    return ret;
                }
            }

            msg_t_del (&_msg);
        }

        tmp = _f (_con, &_msg, ANON_RECV);
        if (tmp < 0)
        {
            ERR_FROM ();
            ret = tmp;
            return ret;
        }

        // message is new
        // todo- make NULL an error
        if (tmp > 0)
        {
            if (_msg->type == MSG_QUIT)
            {
                ret = 0;
                return ret;
            }

            if (_timeout || !_unread)
            {
                tmp = shcon_send_shm_msg (_shcon, _msg);
                if (tmp < 0)
                {
                    ERR_FROM ();
                    ret = tmp;
                    return ret;
                }

                if (_msg->type == MSG_KILL)
                {
                    ret = 0;
                    return ret;
                }

                tmp = shcon_reset_sem_read (_shcon);
                if (tmp < 0)
                {
                    ERR_FROM ();
                    ret = tmp;
                    return ret;
                }

                tmp = shcon_mark_sem_read (_shcon);
                if (tmp < 0)
                {
                    ERR_FROM ();
                    ret = tmp;
                    return ret;
                }
            }
            else
            {
                tmp = _f (_con, &_msg, ANON_PUSH);
                if (tmp < 0)
                {
                    ERR_FROM ();
                    ret = tmp;
                    return ret;
                }
            }
        }

        msg_t_del (&_msg);
        // start at the beginning of the loop from now on
        _create = 0;
    }

    // todo- end of function error
    ret = -1;
    return ret;
}
