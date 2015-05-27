#include <msg.h>

static const msg_hdr_t msg_hdr_empty = { 0 };

msg_t* msg_t_new (void)
{
    // int tmp = 0;
    msg_t* ret = NULL;

    ret = malloc (sizeof (msg_t));
    if (ret == NULL)
    {
        ERR_SYS (errno);
        ERR_PRINT (_EALLOC);
        ret = NULL;
        return ret;
    }

    // todo- test if this is empty
    ret->hdr  = msg_hdr_empty;
    ret->data = NULL;
    ret->cmd  = NULL;
    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}

// todo- create generic set function

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

    if ((*_msg)->cmd != NULL)
    {
        free ((*_msg)->cmd);
    }

    free ((*_msg));
    *(_msg) = NULL;

    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return;
}

void* msg_to_bin (msg_t* _msg)
{
    int   tmp = 0;
    void* ret = NULL;
    int   len = 0;

    if (_msg == NULL || _msg->cmd == NULL || _msg->data == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = NULL;
        return ret;
    }

    tmp = msg_to_bin_len (_msg);
    if (tmp < 0)
    {
        ret = NULL;
        return ret;
    }

    ret = malloc (tmp);
    if (ret == NULL)
    {
        ret = NULL;
        return ret;
    }

    memcpy (ret + len, &(_msg->hdr), sizeof (msg_hdr_t));
    len += sizeof (msg_hdr_t);
    memcpy (ret + len, _msg->cmd, _msg->hdr.cmd_len);
    len += _msg->hdr.cmd_len;
    memcpy (ret + len, _msg->data, _msg->hdr.data_len);
    len += _msg->hdr.data_len;
    if (tmp < 0)
    {
        ret = NULL;
    }
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
    
    ret = sizeof (msg_hdr_t) + _msg->hdr.cmd_len + _msg->hdr.data_len;
    // if (tmp < 0)
    // {
    //     ret = tmp;
    // }
    return ret;
}
