struct shcon_t
{
    //! Common IPC data.
    ipc_t* ipc;
    //! Shared memory to send data.
    shm_t* shm;
    //! Semaphore to lock \b shm and track connections.
    sem_t* sem;
    //! Specifies whether this process has locked the connection.
    int locked;
    //! The time of the previous message recieved.
    int prev_time;
};

extern msg_t shcon_msg_init;
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_ANON_MSG
#define MM_ANON_MSG
enum _ANON_FLAG
{
    ANON_SEND = 0,
    ANON_RECV = 1,
    ANON_PUSH = 2,
};
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

typedef struct shcon_t shcon_t;

int shcon_t_set (shcon_t** _shcon, ipc_t* _ipc, shm_t* _shm, sem_t* _sem);
int shcon_t_from_path (shcon_t** _shcon, char* _path);
void shcon_t_del (shcon_t** _shcon);
int shcon_send_shm_msg (shcon_t* _shcon, msg_t* _msg);
int shcon_send_empty_shm_msg (shcon_t* _shcon);
int shcon_recv_shm_msg (shcon_t* _shcon, msg_t** _msg, int _init);
int shcon_mark_sem_read (shcon_t* _shcon);
int shcon_lock_sem (shcon_t* _shcon);
int shcon_unlock_sem (shcon_t* _shcon);
int shcon_add_sem_con (shcon_t* _shcon);
int shcon_get_sem_con (shcon_t* _shcon);
int shcon_get_sem_read (shcon_t* _shcon);
int shcon_connect (shcon_t* _shcon);
int shcon_msg_loop
  (shcon_t* _shcon, int _create, void* _con, int (*_f) (void*, msg_t**, int));
