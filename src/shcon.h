/** @file shcon.h
@author Roger Bongers
@date May 28 2015
@brief Shared connection operations.
@see shm.h sem.h ipc.h msg.h
**/

#include <ipc.h>
#include <shm.h>
#include <sem.h>
#include <err.h>
#include <msg.h>


/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_SHCON
#define MM_SHCON
enum _SHCON_SEM_SET
{
    SEMSET_LOCK = 0,
    SEMSET_CON  = 1,
    SEMSET_READ = 2,
};

/**
@brief A shared connection.
**/
typedef struct
{
    //! Common IPC data.
    ipc_t* ipc;
    //! Shared memory to send data.
    shm_t* shm;
    //! Semaphore to lock \b shm and track connections.
    sem_t* sem;
    //! Specifies whether this process has locked the connection.
    int locked;
} shcon_t;

extern msg_t shcon_msg_init;
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

/**
@brief Create a new shcon_t.
@return Upon success, returns an address of a shcon_t with empty members.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EALLOC, could not allocate space for the shcon_t.}
@end
**/
shcon_t* shcon_t_new (void);
/**
@brief Populates a shcon_t.
@param _shcon The shared connection to populate.
@param _ipc If \b _ipc is not NULL, populates \b _shcon with the ipc_t address.
If \b _ipc is NULL, populates _shcon member \b ipc with ipc_t_new().
@param _shm If \b _shm is not NULL, populates \b _shcon with the shm address.
If \b _shm is NULL, populates _shcon member \b shm with shm_t_new().
@param _sem If \b _sem is not NULL, populates \b _shcon with the sem_t address.
If \b _sem is NULL, populates _shcon member \b sem with sem_t_new().
@return Upon success, returns 0 and populates \b _shcon.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _sem is NULL.}
@end
@note Inherits errors from ipc_t_new(), shm_t_new(), sem_t_new().
**/
int shcon_t_set (shcon_t** _shcon, ipc_t* _ipc, shm_t* _shm, sem_t* _sem);
/**
@brief Deletes an shcon_t.
@details Assumes that \b _shcon has been properly created by shcon_t_new().
Does nothing if \b _shcon or \b *_shcon is NULL.
@param _shcon The struct to free.
@return Sets \b *_shcon to NULL after freeing data.
**/
void shcon_t_del (shcon_t** _shcon);
int shcon_send_shm_msg (shcon_t* _shcon, msg_t* _msg);
msg_t* shcon_recv_shm_msg (shcon_t* _shcon, int _init);
// todo- make sure you add this to protocol
// todo- define behaviour for marking as read after failures
int shcon_mark_sem (shcon_t* _shcon);
int shcon_lock_sem (shcon_t* _shcon);
int shcon_unlock_sem (shcon_t* _shcon);
int shcon_add_sem_con (shcon_t* _shcon);
// todo- break this up
int shcon_connect (shcon_t* _shcon);
