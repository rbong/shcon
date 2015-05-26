#include <msg.h>

static const msg_hdr_t msg_hdr_empty = { 0 };

msg_t* msg_t_new (void)
{
    msg_t* _msg;

    _msg = malloc (sizeof (msg_t));
    if (_msg == NULL)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_EALLOC);
        return NULL;
    }

    // todo- test if this is empty
    _msg->hdr  = msg_hdr_empty;
    _msg->data = NULL;
    _msg->cmd  = NULL;

    return _msg;
}

void* msg_get_bin (msg_t* _msg)
{
    int   res = 0;
    int   len = 0;
    void* v   = NULL;

    if (_msg == NULL || _msg->cmd == NULL || _msg->data == NULL)
    {
        err_set (_EPTRNULL);
        return NULL;
    }

    res = msg_get_bin_len (_msg);
    if (res < 0)
    {
        return NULL;
    }

    v = malloc (res);
    if (v == NULL)
    {
        return NULL;
    }

    memcpy (v + len, &(_msg->hdr), sizeof (msg_hdr_t));
    len += sizeof (msg_hdr_t);
    memcpy (v + len, _msg->cmd, _msg->hdr.cmd_len);
    len += _msg->hdr.cmd_len;
    memcpy (v + len, _msg->data, _msg->hdr.data_len);
    len += _msg->hdr.data_len;

    return v;
}

int msg_get_bin_len (msg_t* _msg)
{
    int res = 0;

    if (_msg == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }
    
    res += sizeof (msg_hdr_t) + _msg->hdr.cmd_len + _msg->hdr.data_len;

    return res;
}

void msg_t_del (msg_t** _msg)
{
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

    return;
}
