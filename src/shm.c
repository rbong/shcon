/** @file shm.c
@author Roger Bongers
@date May 28 2015
@brief See shm.h.
@see shm.h
**/

#include <shm.h>

//! Default value for shm_t \b size.
int shm_size = 1028;

shm_t* shm_t_new (void)
{
    // int    tmp = 0;
    shm_t* ret = 0;

    ret = malloc (sizeof (shm_t));
    if (ret == NULL)
    {
        ERR_PRINT (_EALLOC);
        return ret;
    }

    ret->size = 0;
    ret->id   = 0;
    ret->seg  = NULL;
    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

int shm_t_set
  (shm_t** _shm, size_t _size, int _id, void* _seg, key_t _key, int _flags)
{
    int tmp = 0;
    int ret = 0;

    if (_shm == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if ((*_shm) == NULL)
    {
        (*_shm) = shm_t_new ();
        if ((*_shm) == NULL)
        {
            ret = -1;
            return ret;
        }
    }

    if (_size == 0)
    {
        // todo- get real max
        (*_shm)->size = shm_size;
    }
    else if (_size > 0)
    {
        (*_shm)->size = _size;
    }

    if (_id == 0 && _key > 0 && _flags >= 0)
    {
        tmp = shm_gen_id ((*_shm), _key, _flags);
        if (tmp < 0)
        {
            if (errno == EEXIST || errno == ENOENT)
            {
                ret = 1;
            }
            else
            {
                ERR_SYS (errno);
                ERR_PRINT (_ESYSTEM);
                ret = -1;
                return ret;
            }
        }
    }
    else if (_id > 0)
    {
        (*_shm)->id = _id;
    }

    if (_seg != NULL)
    {
        (*_shm)->seg = _seg;
    }
    else if ((*_shm)->id > 0)
    {
        tmp = shm_attach_seg (*_shm, NULL, 0);
        if (tmp < 0)
        {
            ret = tmp;
            return ret;
        }
    }

    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

int shm_t_from_id (shm_t** _shm, key_t _key, int _flags)
{
    int tmp = 0;
    int ret = 0;

    tmp = shm_t_set (_shm, 0, 0, NULL, _key, _flags);
    if (tmp != 0)
    {
        ret = tmp;
    }
    return ret;
}

void shm_t_del (shm_t** _shm)
{
    // int tmp = 0;
    // int ret = 0;

    if (_shm == NULL || (*_shm) == NULL)
    {
        return;
    }

    // todo- maybe something for seg

    free (*_shm);
    (*_shm) = NULL;

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return;
}

int shm_gen_id (shm_t* _shm, key_t _key, int _flags)
{
    int tmp     = 0;
    int ret     = 0;

    if (_shm == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    tmp = _shm->id = shmget (_key, _shm->size, _flags);
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

int shm_attach_seg (shm_t* _shm, const void* _shmaddr, int _shmflg)
{
    // int tmp = 0;
    int ret = 0;

    if (_shm == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    _shm->seg = shmat (_shm->id, _shmaddr, _shmflg);
    if (_shm->seg < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
        ret = -1;
    }
    return ret;
}

int shm_read (shm_t* _shm, void* _buf, int _bytes, int _offset)
{
    // int tmp = 0;
    int ret = 0;

    if (_shm == NULL || _buf == NULL || _shm->seg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if (_offset < 0 || _bytes < 0 || _offset + _bytes > _shm->size)
    {
        ERR_PRINT (_EBADVAL);
        ret = -1;
        return ret;
    }

    memcpy (_buf, _shm->seg + _offset, _bytes);
    if (_buf == NULL)
    {
        ERR_PRINT (_ESYSTEM);
        ret = -1;
    }
    return ret;
}

int shm_write (shm_t* _shm, void* _buf, int _bytes, int _offset)
{
    // int tmp = 0;
    int ret = 0;

    if (_shm == NULL || _buf == NULL || _shm->seg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if (_offset < 0 || _bytes < 0 || _offset + _bytes > _shm->size)
    {
        ERR_PRINT (_EBADVAL);
        ret = -1;
        return ret;
    }

    memcpy (_shm->seg + _offset, _buf, _bytes);
    if (_shm->seg == NULL)
    {
        ERR_PRINT (_ESYSTEM);
        ret = -1;
    }
    return ret;
}
