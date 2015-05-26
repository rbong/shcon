#include <msg.h>

static const msg_hdr_t msg_hdr_empty = { 0 };

msg_t* msg_t_new (void)
{
    msg_t* msg;

    msg = malloc (sizeof (msg_t));
    if (msg == NULL)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_EALLOC);
        return NULL;
    }

    // todo- test if this is empty
    msg->hdr  = msg_hdr_empty;
    msg->data = NULL;
    msg->cmd  = NULL;

    return msg;
}

void* msg_get_bin (msg_t* msg)
{
    int   res = 0;
    int   len = 0;
    void* v   = NULL;

    if (msg == NULL || msg->cmd == NULL || msg->data == NULL)
    {
        err_set (_EPTRNULL);
        return NULL;
    }

    res = msg_get_bin_len (msg);
    if (res < 0)
    {
        return NULL;
    }

    v = malloc (res);
    if (v == NULL)
    {
        return NULL;
    }

    memcpy (v + len, &(msg->hdr), sizeof (msg_hdr_t));
    len += sizeof (msg_hdr_t);
    memcpy (v + len, msg->cmd, msg->hdr.cmd_len);
    len += msg->hdr.cmd_len;
    memcpy (v + len, msg->data, msg->hdr.data_len);
    len += msg->hdr.data_len;

    return v;
}

int msg_get_bin_len (msg_t* msg)
{
    int res = 0;

    if (msg == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }
    
    res += sizeof (msg_hdr_t) + msg->hdr.cmd_len + msg->hdr.data_len;

    return res;
}

void msg_t_del (msg_t** msg)
{
    if (msg == NULL || (*msg) == NULL)
    {
        return;
    }

    if ((*msg)->data != NULL)
    {
        free ((*msg)->data);
    }

    if ((*msg)->cmd != NULL)
    {
        free ((*msg)->cmd);
    }

    free ((*msg));
    *msg = NULL;

    return;
}
