#include <msg.h>

// todo- check if all _t_new values for NULL
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

    msg->version  = 0;
    msg->date     = 0;
    msg->cmd_len  = 0;
    msg->data_len = 0;
    msg->data     = NULL;
    msg->cmd      = NULL;

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

    // todo- split this up
    memcpy (v + len, &(msg->version), sizeof (msg->version));
    len += sizeof (msg->version);
    memcpy (v + len, &(msg->date), sizeof (msg->date));
    len += sizeof (msg->date);
    memcpy (v + len, &(msg->cmd_len), sizeof (msg->cmd_len));
    len += sizeof (msg->cmd_len);
    memcpy (v + len, &(msg->data_len), sizeof (msg->data_len));
    len += sizeof (msg->data_len);
    memcpy (v + len, msg->cmd, msg->cmd_len);
    len += msg->cmd_len;
    memcpy (v + len, msg->data, msg->data_len);
    len += msg->data_len;

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
    
    res += sizeof (msg->version) + sizeof (msg->date) +
        sizeof (msg->cmd_len) + sizeof (msg->data_len);
    res += msg->cmd_len + msg->data_len;

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
