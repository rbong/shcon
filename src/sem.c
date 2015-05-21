#include <sem.h>

int sem_len = 1;

// todo- keep the return values the same, but make struct an arg to safely
// detect when allocated memory is not being captured. do this for all _new
sem_t* sem_t_new (void)
{
    sem_t* sem = NULL;

    sem = malloc (sizeof (sem_t));

    if (sem == NULL)
    {
        err_set (_EALLOC);
        return NULL;
    }

    sem->ipc = NULL;
    sem->len = 0;
    sem->id  = 0;

    return sem;
}

int sem_t_set (sem_t** sem, ipc_t* _ipc, int _len, int _id)
{
    int res = 0;
    int ret = 0;

    if ((*sem) == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if ((*sem) == NULL)
    {
        (*sem) = sem_t_new ();
        if ((*sem) == NULL)
        {
            return -1;
        }
    }

    if (_ipc != NULL)
    {
        (*sem)->ipc = _ipc;
    }

    if (_len == 0)
    {
        (*sem)->len = sem_len;
    }
    else if (_len > 0)
    {
        (*sem)->len = _len;
    }

    if (_id == 0)
    {
        (*sem)->id = sem_gen_id ((*sem));
        if ((*sem)->id < 0)
        {
            return (*sem)->id;
        }
    }
    else if (_id > 0)
    {
        (*sem)->id = _id;
    }


    // todo- remove newlines from related error statements
    if (res < 0)
    {
        ret = res;
    }
    return ret;
}

void sem_t_del (sem_t* sem)
{
    if (sem == NULL)
    {
        return;
    }

    if (sem->ipc != NULL)
    {
        free (sem->ipc);
    }

    free (sem);

    return;
}

// todo- determine if this should allocate or not
int sem_gen_id (sem_t* sem)
{
    if (sem == NULL || sem->ipc == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    sem->id = semget (sem->ipc->key, sem->len, sem->ipc->flags);

    if (sem->id < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        // todo- make a new error
        err_set (_ESYSTEM);
        return -1;
    }

    return 0;
}

int sem_lock (sem_t* sem)
{
    int res            = 0;
    int ret            = 0;
    struct sembuf lock = { 0 };

    if (sem == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    // todo- make this its own file if it gets too complex
    res = semctl (sem->id, 0, GETVAL, 0);
    if (res <= 0)
    {
        // todo- make warning
        return 0;
    }

    lock.sem_num = 0;
    lock.sem_op  = -1;
    lock.sem_flg = IPC_NOWAIT;

    res = semop (sem->id, &lock, sem->len);
    if (res < 0)
    {
        ret = res;
    }

    return ret;
}

int sem_unlock (sem_t* sem)
{
    int res            = 0;
    int ret            = 0;
    struct sembuf lock = { 0 };

    if (sem == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    // todo- make this its own file if it gets too complex
    res = semctl (sem->id, 0, GETVAL, 0);
    if (res >= 1)
    {
        // todo- make warning
        return 0;
    }

    lock.sem_num = 0;
    lock.sem_op  = 1;
    lock.sem_flg = IPC_NOWAIT;


    res = semop (sem->id, &lock, sem->len);
    if (res < 0)
    {
        ret = res;
    }

    return ret;
}
