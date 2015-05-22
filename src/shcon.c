#include <shcon.h>

// todo- set all local variable names to have underscores in ipc.c and sem.c
shcon_t* shcon_t_new (void)
{
    shcon_t* _shcon = NULL;

    _shcon = malloc (sizeof (shcon_t));

    if (_shcon == NULL)
    {
        err_set (_EALLOC);
        return NULL;
    }

    _shcon->ipc = NULL;
    _shcon->shm = NULL;
    _shcon->sem = NULL;

    return _shcon;
}

int shcon_t_from_path (shcon_t** _shcon, char* root, char* sub)
{
    int res = 0;

    res = ipc_t_from_path (&((*_shcon)->ipc), root, sub);

    if (res < 0)
    {
        return res;
    }

    res = shm_t_from_ipc (&((*_shcon)->shm), (*_shcon)->ipc);

    if (res < 0)
    {
        return res;
    }

    res = sem_t_from_ipc (&((*_shcon)->sem), (*_shcon)->ipc);

    if (res < 0)
    {
        return res;
    }

    return res;
}

void shcon_t_del (shcon_t** _shcon)
{
    if (_shcon == NULL || *_shcon == NULL)
    {
        return;
    }

    if ((*_shcon)->ipc != NULL)
    {
        free ((*_shcon)->ipc);
    }

    if ((*_shcon)->shm != NULL)
    {
        free ((*_shcon)->shm);
    }

    if ((*_shcon)->sem != NULL)
    {
        free ((*_shcon)->sem);
    }

    return;
}
