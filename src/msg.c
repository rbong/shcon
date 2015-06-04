/** @file msg.c
@author Roger Bongers
@date June 2 2015
@brief See msg.h.
@see msg.h
**/

#include <msg.h>

//! Default value for msg_t#type.
long msg_type = MSG_NORM;

msg_t* msg_t_new (void)
{
    // int tmp = 0;
    msg_t* ret = NULL;

    ret = malloc (sizeof (msg_t));
    if (ret == NULL)
    {
        ERR_PRINT (_EALLOC);
        return ret;
    }

    ret->type = 0;
    ret->hdr  = (msg_hdr_t) { 0 };
    ret->data = NULL;
    // if (tmp < 0)
    // {
    //     ret = NULL;
    // }
    return ret;
}

int msg_t_set (msg_t** _msg, long _type, msg_hdr_t _hdr, char* _data)
{
    int tmp = 0;
    int ret = 0;

    if (_msg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    if ((*_msg) == NULL)
    {
        (*_msg) = msg_t_new ();
        if ((*_msg) == NULL)
        {
            ret = -1;
            return ret;
        }
    }

    if (_type == 0)
    {
        (*_msg)->type = msg_type;
    }
    else if (_type > 0)
    {
        (*_msg)->type = _type;
    }

    if (_hdr.ver == 0)
    {
        (*_msg)->hdr = msg_gen_hdr ();
        if ((*_msg)->hdr.ver == 0)
        {
            ret = -1;
            return ret;
        }
    }
    else if (_hdr.ver > 0)
    {
        (*_msg)->hdr = _hdr;
    }

    if (_data != NULL)
    {
        tmp = msg_set_data (*_msg, _data);
    }
    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

void msg_t_del (msg_t** _msg)
{
    // int tmp = 0;
    // int ret = 0;

    if (_msg == NULL || (*_msg) == NULL)
    {
        return;
    }

    if ((*_msg)->data != NULL)
    {
        free ((*_msg)->data);
    }

    free (*_msg);

    *_msg = NULL;

    // if (tmp < 0)
    // {
    //     ret = NULL;
    // }
    // return ret;
}

msg_hdr_t msg_gen_hdr (void)
{
    // int tmp = 0;
    msg_hdr_t ret = { 0 };

    ret.ver = MM_HDR_VER;
    ret.date = get_sec ();
    if (ret.date < 0)
    {
        ret.ver = 0;
    }
    ret.len = 0;

    return ret;
}

int msg_set_data (msg_t* _msg, char* _data)
{
    // int tmp = 0;
    int ret = 0;
    int _len = 0;

    if (_msg == NULL || _data == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    _len = strlen (_data) + 1;
    _msg->data = malloc (_len);
    if (_msg->data == NULL)
    {
        ERR_PRINT (_EALLOC);
        ret = -1;
        return ret;
    }
    strcpy (_msg->data, _data);
    _msg->hdr.len = _len;

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

void* msg_to_bin (msg_t* _msg)
{
    int tmp = 0;
    void* ret = NULL;
    int _len = 0;
    int _offset = 0;

    if (_msg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = NULL;
        return ret;
    }

    tmp = _len = msg_to_bin_len (_msg);
    if (tmp < 0)
    {
        ret = NULL;
        return ret;
    }

    ret = malloc (_len);
    if (ret == NULL)
    {
        ERR_PRINT (_EALLOC);
        return ret;
    }

    memcpy (ret + _offset, &(_msg->type), sizeof (_msg->type));
    _offset += sizeof (_msg->type);
    memcpy (ret + _offset, &(_msg->hdr), sizeof (_msg->hdr));
    _offset += sizeof (_msg->hdr);
    memcpy (ret + _offset, _msg->data, _msg->hdr.len);
    return ret;
}

msg_t* msg_from_bin (void* _bmsg)
{
    // int tmp = 0;
    msg_t* ret = msg_t_new ();
    int _len = 0;
    int _offset = 0;

    if (ret == NULL)
    {
        return ret;
    }

    if (_bmsg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = NULL;
        return ret;
    }

    _len =  sizeof (_bmsg);
    _len -= sizeof (ret->type);
    _len -= sizeof (ret->hdr);
    if (_len > 1)
    {
        ret->data = malloc (_len);
        if (ret->data == NULL)
        {
            ERR_SYS (errno);
            ERR_PRINT (_EALLOC);
            msg_t_del (ret);
            return ret;
        }
    }

    memcpy (&(ret->type), _bmsg + _offset, sizeof (ret->type));
    _offset += sizeof (ret->type);
    memcpy (&(ret->hdr), _bmsg + _offset, sizeof (ret->hdr));
    _offset += sizeof (ret->hdr);
    memcpy (ret->data, _bmsg + _offset, _len);
    return ret;
}

int msg_to_bin_len (msg_t* _msg)
{
    // int tmp = 0;
    int ret = 0;

    if (_msg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    ret = sizeof (_msg->type) + sizeof (_msg->hdr) + _msg->hdr.len;
    return ret;
}
