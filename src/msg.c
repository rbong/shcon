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

    msg->date     = 0;
    msg->cmd_len  = 0;
    msg->data_len = 0;
    msg->data     = NULL;
    msg->cmd      = NULL;

    return msg;
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
