#include <ipc.h>

int   ipc_flags   = IPC_USR_R | IPC_USR_W | IPC_GRP_R |
                    IPC_GRP_W | IPC_CREAT;
int   ipc_proj_id = 'M';
char* ipc_root    = "/tmp/";
char* ipc_sub     = "mm";

key_t (*ipc_gen_key) (char*, int) = ipc_gen_key_ftok;

ipc_t* ipc_t_new (void)
{
    ipc_t* ipc = NULL;

    ipc = malloc (sizeof (ipc_t));

    if (ipc == NULL)
    {
        err_set (_EALLOC);
        return NULL;
    }

    ipc->path    = NULL;
    ipc->flags   = 0;
    ipc->proj_id = 0;
    ipc->key     = 0;

    return ipc;
}

int ipc_t_set (ipc_t** ipc, int flags, int proj_id, char* path, key_t key)
{
    int res = 0;
    int ret = 0;

    if (ipc == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if ((*ipc) == NULL)
    {
        (*ipc) = ipc_t_new ();

        if ((*ipc) == NULL)
        {
            return -1;
        }
    }

    if (flags == 0)
    {
        (*ipc)->flags = ipc_flags;
    }
    else if (flags > 0)
    {
        (*ipc)->flags = flags;
    }

    if (proj_id == 0)
    {
        (*ipc)->proj_id = ipc_proj_id;
    }
    else if (proj_id > 0)
    {
        (*ipc)->proj_id = proj_id;
    }

    if (path != NULL)
    {
        (*ipc)->path = path;
    }

    if (key == 0)
    {
        (*ipc)->key = ipc_gen_key ((*ipc)->path, (*ipc)->proj_id);

        if (res < 0)
        {
            ret = res;
        }
    }
    else if (key > 0)
    {
        (*ipc)->key = key;
    }


    if (res < 0)
    {
        ret = res;
    }

    return ret;
}

int ipc_t_set_from_path (ipc_t** ipc, char* root, char* sub)
{
    char* path = NULL;

    path = ipc_gen_path (root, sub);

    if (path == NULL)
    {
        return -1;
    }

    return ipc_t_set (ipc, 0, 0, path, 0);
}

void ipc_t_del (ipc_t* ipc)
{
    if (ipc == NULL)
    {
        return;
    }

    if (ipc->path != NULL)
    {
        free (ipc->path);
    }

    free (ipc);
}

char* ipc_gen_path (char* root, char* sub)
{
    int   ret    = NULL;
    char* path   = NULL;
    char* as [2] = { NULL, NULL };

    err_reset ();

    if (sub == NULL)
    {
        err_set (_EPTRNULL);
        return NULL;
    }

    if (sub [0] == '\0')
    {
        err_set (_ESTREMPTY);
        return NULL;
    }

    if (root == NULL || root [0] == '\0')
    {
        root = ipc_root;
    }

    as [0] = root;
    as [1] = sub;
    ret = str_cat_len (2, as);

    if (ret <= 0)
    {
        return NULL;
    }

    path = malloc (ret);

    if (path == NULL)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_EALLOC);
        return NULL;
    }

    ret = str_cat (2, path, as);

    if (ret < 0)
    {
        return NULL;
    }
    if (path == NULL)
    {
        err_set (_EBADFUNC);
        return NULL;
    }

    ret = file_touch (path);

    if (ret < 0)
    {
        free (path);
        return NULL;
    }

    return path;
}

key_t ipc_gen_key_ftok (char* path, int proj_id)
{
    key_t key = 0;

    err_reset ();

    if (path == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if (proj_id <= 0)
    {
        err_set (_EBADVAL);
        return -1;
    }

    key = ftok (path, proj_id);

    if (key < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_ESYSTEM);
        return -1;
    }

    return key;
}
