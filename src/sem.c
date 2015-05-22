// todo- sort out return values
#include <sem.h>

int sem_add_conn (sem_t*);

int           sem_len        = 2;
struct sembuf sem_lock_buf   = { 1, -1, SEM_UNDO };
struct sembuf sem_unlock_buf = { 1, +1, IPC_NOWAIT };

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

    sem->ipc    = NULL;
    sem->len    = 0;
    sem->id     = 0;
    sem->locked = 0;

    return sem;
}

int sem_t_set (sem_t** sem, ipc_t* _ipc, int _id)
{
    int res = 0;
    int ret = 0;

    if (sem == NULL)
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

    (*sem)->len = sem_len;

    if (_id == 0)
    {
        res = sem_gen_id ((*sem));
        if (res < 0)
        {
            return res;
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

// todo- check consistency between similar structure functions
int sem_t_from_ipc (sem_t** _sem, ipc_t* _ipc)
{
    int res = 0;
    int ret = 0;

    if (_ipc == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    res = sem_t_set (_sem, _ipc, 0);

    if (res < 0)
    {
        ret = res;
    }
    return ret;
}

// todo- make delete functions accept double pointers to set values to NULL
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

// todo- determine if this should allocate or not, split this up
int sem_gen_id (sem_t* sem)
{
    int         ret    = 0;
    int         _flags = 0;
    union semun sem_ops;

    if (sem == NULL || sem->ipc == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    // create new semaphore if it doesn't exist
    _flags = sem->ipc->flags | IPC_CREAT | IPC_EXCL;
    sem->id = semget (sem->ipc->key, sem->len, _flags);
    if (sem->id < 0)
    {
        switch (errno)
        {
            case EEXIST:
                err_reset ();
                break;
            default:
                ERR_AT_LINE_SYS (0, errno);
                err_set (_ESYSTEM);
                ret = sem->id;
                return ret;
        }
    }
    else
    {
        // todo- add enums to indicate what semaphore you're on
        sem_ops.val = 0;
        ret = semctl (sem->id, 0, SETVAL, sem_ops);
        if (ret < 0)
        {
            ERR_AT_LINE_SYS (0, errno);
            err_set (_ESYSTEM);
            return ret;
        }

        sem_ops.val = 1;
        ret = semctl (sem->id, 1, SETVAL, sem_ops);
        if (ret < 0)
        {
            ERR_AT_LINE_SYS (0, errno);
            err_set (_ESYSTEM);
            return ret;
        }

        ret = sem_add_conn (sem);
        if (ret < 0)
        {
            ERR_AT_LINE_SYS (0, errno);
            err_set (_ESYSTEM);
            return ret;
        }

        return ret;
    }

    sem->id = semget (sem->ipc->key, sem->len, sem->ipc->flags);

    if (sem->id < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        // todo- make a new error
        err_set (_ESYSTEM);
        return -1;
    }

    ret = sem_add_conn (sem);
    if (ret < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_ESYSTEM);
        return ret;
    }

    return 0;
}

// todo- test race conditions, read more on semop
int sem_lock (sem_t* sem)
{
    if (sem == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if (sem->locked)
    {
        return 1;
    }

    if (semop (sem->id, &sem_lock_buf, 1) < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_ESYSTEM);
        return -1;
    }

    sem->locked = 1;

    return 0;
}

int sem_unlock (sem_t* sem)
{
    if (sem == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    if (!(sem->locked))
    {
        return 1;
    }

    if (semop (sem->id, &sem_unlock_buf, 1) < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_ESYSTEM);
        return -1;
    }

    sem->locked = 0;

    return 0;
}

// do not use this external to sem
int sem_add_conn (sem_t* sem)
{
    int           res =   0;
    struct sembuf add = { 0 };

    if (sem == NULL)
    {
        err_set (_EPTRNULL);
        return -1;
    }

    add.sem_num = 0;
    add.sem_op  = 1;
    add.sem_flg = IPC_NOWAIT | SEM_UNDO;

    res = semop (sem->id, &add, 1);
    if (res < 0)
    {
        ERR_AT_LINE_SYS (0, errno);
        err_set (_ESYSTEM);
    }
    return res;
}
