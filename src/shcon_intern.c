/** @file shcon_intern.c
@author Roger Bongers
@date June 2 2015
@brief See shcon_intern.h.
@see shcon_intern.h
**/

#include <shcon_intern.h>

int _shcon_wait_ms = 100;

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
        err_num = _ENOBLAME;
        ERR_FROM ();
        ret = tmp;
        return ret;
    }
    _shcon->locked = 1;

    tmp = sem_ctl (_shcon->sem, SEMSET_CON,  SETVAL, (union sem_un) 0);
    if (tmp < 0)
    {
        err_num = _ENOBLAME;
        ERR_FROM ();
        ret = tmp;
        return ret;
    }

    tmp = sem_ctl (_shcon->sem, SEMSET_READ,  SETVAL, (union sem_un) 0);
    if (tmp < 0)
    {
        err_num = _ENOBLAME;
        ERR_FROM ();
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
            err_num = _ENOBLAME;
            ERR_FROM ();
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
    }
    else
    {
        tmp = shm_attach_seg (_shcon->shm, NULL, 0);
        if (tmp < 0)
        {
            err_num = _ENOBLAME;
            ERR_FROM ();
            ret = tmp;
        }
    }
    return ret;
}

int _shcon_init_shm (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;

    if (_shcon == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    tmp = shcon_send_shm_msg (_shcon, &shcon_msg_init);
    if (tmp < 0)
    {
        err_num = _ENOBLAME;
        ERR_FROM ();
        ret = tmp;
    }

    tmp = shcon_send_empty_shm_msg (_shcon);
    if (tmp < 0)
    {
        err_num = _ENOBLAME;
        ERR_FROM ();
        ret = tmp;
    }
    
    return ret;
}

int _shcon_check_shm_ver (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    msg_t* _msg;

    if (_shcon == NULL)
    {
        err_num = _EPTRNULL;
        ret = -1;
        return ret;
    }

    tmp = shcon_recv_shm_msg (_shcon, &_msg, 1);
    if (tmp < 0)
    {
        err_num = _ENOBLAME;
        ERR_FROM ();
        ret = -1;
        return ret;
    }

    if (_msg->hdr.ver != MM_HDR_VER)
    {
        // todo- new error
        err_num = _EBADVAL;
        ret = -1;
        return ret;
    }
    msg_t_del (&_msg);
    return ret;
}

// todo- make this non-shcon specific
int _shcon_wait (int _ms)
{
    int tmp = 0;
    int ret = 0;

    if (_ms < 1)
    {
        _ms = _shcon_wait_ms;
    }

    tmp = usleep (_ms * 1000);
    if (tmp < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
        ret = tmp;
    }
    return ret;
}

// todo- make this non-shcon specific
int _shcon_timed_out_msg (msg_t* _msg)
{
    int tmp = 0;
    int ret = 0;
    int _cur_time = 0;

    if (_msg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    tmp = _cur_time = get_sec ();
    if (tmp < 0)
    {
        ERR_FROM ();
        ret = tmp;
        return ret;
    }

    tmp = (_msg->hdr.timeout + _msg->hdr.date);

    ret = tmp < _cur_time;
    return ret;
}

// todo- add retry capability
int _shcon_has_unread_sem (shcon_t* _shcon)
{
    int tmp = 0;
    int ret = 0;
    int _cons = 0;
    int _reads = 0;
    int _prev_cons = 0;

    tmp = _cons = shcon_get_sem_con (_shcon);
    if (tmp < 0)
    {
        ERR_FROM ();
        ret = tmp;
        return ret;
    }

    tmp = _reads = shcon_get_sem_read (_shcon);
    if (tmp < 0)
    {
        ERR_FROM ();
        ret = tmp;
        return ret;
    }

    _prev_cons = _cons;

    tmp = _cons = shcon_get_sem_con (_shcon);
    if (tmp < 0)
    {
        ERR_FROM ();
        ret = tmp;
        return ret;
    }

    // changes in connections since the last check- assum there's unread
    if (_cons != _prev_cons)
    {
        ret = 1;
        return ret;
    }

    ret = (_reads < _cons);
    return ret;
}

int _shcon_check_prev_time (shcon_t* _shcon, msg_t* _msg)
{
    // int tmp = 0;
    int ret = 0;

    if (_shcon == NULL || _msg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
    }

    if (_shcon->prev_time == 0 || _shcon->prev_time < _msg->hdr.date)
    {
        ret = 1;
    }
    return ret;
}

int _shcon_set_prev_time (shcon_t* _shcon, msg_t* _msg)
{
    // int tmp = 0;
    int ret = 0;

    if (_shcon == NULL || _msg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
    }
    
    _shcon->prev_time = _msg->hdr.date;

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}
