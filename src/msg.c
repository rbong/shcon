/** @file msg.c
@author Roger Bongers
@date June 2 2015
@brief See msg.h.
@see msg.h
**/

#include <msg.h>

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
