/** @file ipc.c
@author Roger Bongers
@date May 27 2015
@brief See ipc.h.
@see ipc.h
**/

#include <ipc.h>

//! Default value for ipc_t#flags.
int   ipc_flags   = IPC_USR_R | IPC_USR_W | IPC_GRP_R | IPC_GRP_W;
//! Default value for ipc_t#proj_id.
int   ipc_proj_id = 'M';

key_t (*ipc_gen_key) (char*, int) = ipc_gen_key_ftok;

ipc_t* ipc_t_new (void)
{
    // int tmp = 0;
    ipc_t* ret = NULL;

    ret = malloc (sizeof (ipc_t));
    if (ret == NULL)
    {
        ERR_PRINT (_EALLOC);
        ret = NULL;
        return ret;
    }

    ret->path    = NULL;
    ret->flags   = 0;
    ret->proj_id = 0;
    ret->key     = 0;
    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

int ipc_t_set (ipc_t** _ipc, int _flags, char* _path, key_t _key)
{
    int tmp = 0;
    int ret = 0;

    if (_ipc == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if ((*_ipc) == NULL)
    {
        (*_ipc) = ipc_t_new ();
        if ((*_ipc) == NULL)
        {
            ERR_FROM ();
            ret = -1;
            return ret;
        }
    }

    if (_flags == 0)
    {
        (*_ipc)->flags = ipc_flags;
    }
    else if (_flags > 0)
    {
        (*_ipc)->flags = _flags;
    }

    (*_ipc)->proj_id = ipc_proj_id;

    if (_path != NULL)
    {
        (*_ipc)->path = _path;
    }

    if (_key == 0 && (*_ipc)->path != NULL && (*_ipc)->proj_id > 0)
    {
        tmp = (*_ipc)->key = ipc_gen_key ((*_ipc)->path, (*_ipc)->proj_id);
        if (tmp < 0)
        {
            ERR_FROM ();
            ret = tmp;
        }
    }
    else if (_key > 0)
    {
        (*_ipc)->key = _key;
    }
    return ret;
}

int ipc_t_from_path (ipc_t** _ipc, char* _path)
{
    int   tmp   = 0;
    int   ret   = 0;

    if (_path == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    tmp = ipc_t_set (_ipc, 0, _path, 0);
    if (tmp < 0)
    {
        ERR_FROM ();
        ret = tmp;
    }
    return ret;
}

void ipc_t_del (ipc_t** _ipc)
{
    // int tmp = 0;
    // int ret = 0;

    if (_ipc == NULL || (*_ipc) == NULL)
    {
        return;
    }

    if ((*_ipc)->path != NULL)
    {
        free ((*_ipc)->path);
    }

    free ((*_ipc));
    (*_ipc) = NULL;

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return;
}

key_t ipc_gen_key_ftok (char* _path, int _proj_id)
{
    // int tmp = 0;
    key_t ret = 0;

    if (_path == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if (_proj_id <= 0)
    {
        ERR_PRINT (_EBADVAL);
        ret = -1;
        return ret;
    }

    ret = ftok (_path, _proj_id);
    if (ret < 0)
    {
        ERR_SYS (errno);
        // todo- make a new error
        ERR_PRINT (_ESYSTEM);
        ret = -1;
        return ret;
    }
    return ret;
}
