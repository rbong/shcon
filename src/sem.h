struct sem_t
{
    //! The number of semaphores in the set.
    int    len;
    //! Semaphore set identifier.
    int    id;
};

//! Type of the optional parameter for \b semctl() as specified by POSIX.
union sem_un
{
    int              val;
    struct semid_ds* buf;
    unsigned short*  array;
};
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

typedef struct sem_t sem_t;

int sem_t_set (sem_t** _sem, int _len, int _id, key_t _key, int _flags);
int sem_t_from_id (sem_t** _sem, key_t _key, int _flags);
void sem_t_del (sem_t** _sem);
int sem_gen_id (sem_t* _sem, key_t _key, int _flags);
int sem_ctl (sem_t* _sem, int _sem_num, int _cmd, union sem_un _sem_un);
int sem_op (sem_t* _sem, struct sembuf _sem_ops, size_t _num_sem_ops);
