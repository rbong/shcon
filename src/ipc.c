#include <ipc.h>

int   ipc_flags   = IPC_USR_R | IPC_USR_W | IPC_GRP_R | IPC_GRP_W;
int   ipc_proj_id = 'M';
char* ipc_root    = "/tmp/";
char* ipc_sub     = "mm";

key_t (*ipc_gen_key) (char*, int) = ipc_gen_key_ftok;

ipc_t* ipc_t_new (void)
{
    ipc_t* _ipc = NULL;

    _ipc = malloc (sizeof (ipc_t));
    if (_ipc == NULL)
    {
        ERR_PRINT (_EALLOC);
        return NULL;
    }

    _ipc->path    = NULL;
    _ipc->flags   = 0;
    _ipc->proj_id = 0;
    _ipc->key     = 0;
    return _ipc;
}

int ipc_t_set (ipc_t** _ipc, int _flags, int _proj_id, char* _path, key_t _key)
{
    int tmp = 0;
    int ret = 0;

    if (_ipc == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    if ((*_ipc) == NULL)
    {
        (*_ipc) = ipc_t_new ();
        if ((*_ipc) == NULL)
        {
            return -1;
        }
    }

    if (_flags == 0)
    {
        (*_ipc)->flags = ipc_flags;
    }
    else if (_flags > 0)
    {
        // IPC_EXCL and IPC_CREAT should only be used by the program
        (*_ipc)->flags = _flags & (~IPC_EXCL) & (~IPC_CREAT);
    }

    if (_proj_id == 0)
    {
        (*_ipc)->proj_id = ipc_proj_id;
    }
    else if (_proj_id > 0)
    {
        (*_ipc)->proj_id = _proj_id;
    }

    if (_path != NULL)
    {
        (*_ipc)->path = _path;
    }

    if (_key == 0)
    {
        tmp = (*_ipc)->key = ipc_gen_key ((*_ipc)->path, (*_ipc)->proj_id);
        if (tmp < 0)
        {
            ret = tmp;
        }
    }
    else if (_key > 0)
    {
        (*_ipc)->key = _key;
    }

    if (tmp < 0)
    {
        ret = tmp;
    }
    return ret;
}

int ipc_t_from_path (ipc_t** _ipc, char* _root, char* _sub)
{
    int   tmp   = 0;
    char* _path = NULL;

    // all NULL checks are performed by called functions

    _path = ipc_gen_path (_root, _sub);
    if (_path == NULL)
    {
        return -1;
    }

    tmp = ipc_t_set (_ipc, 0, 0, _path, 0);
    return tmp;
}

void ipc_t_del (ipc_t** _ipc)
{
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

    return;
}

char* ipc_gen_path (char* _root, char* _sub)
{
    int   ret    = NULL;
    char* _path   = NULL;
    char* as [2] = { NULL, NULL };

    if (_sub == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return NULL;
    }

    if (_sub [0] == '\0')
    {
        ERR_PRINT (_ESTREMPTY);
        return NULL;
    }

    if (_root == NULL || _root [0] == '\0')
    {
        _root = ipc_root;
    }

    as [0] = _root;
    as [1] = _sub;
    ret = str_cat_len (2, as);
    if (ret <= 0)
    {
        return NULL;
    }

    _path = malloc (ret);
    if (_path == NULL)
    {
        ERR_SYS (errno);
        ERR_PRINT (_EALLOC);
        return NULL;
    }

    ret = str_cat (2, _path, as);
    if (ret < 0)
    {
        return NULL;
    }
    if (_path == NULL)
    {
        ERR_PRINT (_EBADFUNC);
        return NULL;
    }

    ret = file_touch (_path);
    if (ret < 0)
    {
        free (_path);
        return NULL;
    }
    return _path;
}

key_t ipc_gen_key_ftok (char* _path, int _proj_id)
{
    key_t key = 0;

    if (_path == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        return -1;
    }

    if (_proj_id <= 0)
    {
        ERR_PRINT (_EBADVAL);
        return -1;
    }

    key = ftok (_path, _proj_id);
    if (key < 0)
    {
        ERR_SYS (errno);
        ERR_PRINT (_ESYSTEM);
        return -1;
    }
    return key;
}
