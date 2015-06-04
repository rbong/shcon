/** @file shcon.h
@author Roger Bongers
@date May 28 2015
@brief Shared connection operations.
@section shcon_connect_protocol Connection protocol
The following is an illustration of the connection protocol.
Fatal errors are not included.
Click the boxes to view the corresponding function.
@dotfile shcon_connect.dot
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
If \b _ipc is NULL, populates _shcon shcon_t#ipc with ipc_t_new().
@param _shm If \b _shm is not NULL, populates \b _shcon with the shm address.
If \b _shm is NULL, populates _shcon shcon_t#shm with shm_t_new().
@param _sem If \b _sem is not NULL, populates \b _shcon with the sem_t address.
If \b _sem is NULL, populates _shcon shcon_t#sem with sem_t_new().
@return Upon success, returns 0 and populates \b _shcon.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _sem is NULL.}
@end
@note Inherits errors from ipc_t_new(), shm_t_new(), sem_t_new().
**/
int shcon_t_set (shcon_t** _shcon, ipc_t* _ipc, shm_t* _shm, sem_t* _sem);
/**
@brief Deletes a shcon_t.
@details Assumes that \b _shcon has been properly created by shcon_t_new().
Does nothing if \b _shcon or \b *_shcon is NULL.
@param _shcon The struct to free.
@return Sets \b *_shcon to NULL after freeing data.
**/
void shcon_t_del (shcon_t** _shcon);
/**
@brief Sends a message to shared memory.
@details If the message is not an init message,
locks the thread until the shared connection is available.
@param _shcon The shared connection with the shared memory.
@param _msg The message to send.
@return Upon success, returns 0.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#shm  are NULL}
@end
@note Inherits errors from msg_to_bin(), shcon_lock_sem(), shm_write(),
shcon_unlock_sem().
**/
int shcon_send_shm_msg (shcon_t* _shcon, msg_t* _msg);
/**
@brief Gets a message from shared memory.
@details Locks the thread until the shared connection is available.
@param _shcon The shared connection with the shared memory.
@param _init Indicates to read the header message instead of post-header.
@return Upon success, returns a newly allocated message.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b l_shcon shcon_t#shm  are NULL}
@end
@note Inherits errors from msg_t_new(), msg_to_bin_len(), shm_read().
**/
msg_t* shcon_recv_shm_msg (shcon_t* _shcon, int _init);
/**
@brief Marks the current message as read on a semaphore.
@details The action will be undone when the thread disconnects.
@param _shcon The shared connection with the semaphore.
@return Upon success, returns 0.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#shm  are NULL}
@end
@note Inherits errors from sem_op().
**/
int shcon_mark_sem (shcon_t* _shcon);
/**
@brief Locks a semapohre.
@details Locks the thread until the shared connection is available.
@param _shcon The shared connection with the semaphore to lock.
@return Upon success, returns 0 and sets \b _shcon shcon_t#locked to 1.
<br>If the shared connection was already locked by the thread, returns 1.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#sem  are NULL}
@end
@note Inherits errors from sem_op().
**/
int shcon_lock_sem (shcon_t* _shcon);
/**
@brief Unlocks a semapohre.
@param _shcon The shared connection with the semaphore to lock.
@return Upon success, returns 0 and sets \b _shcon shcon_t#locked to 0.
<br>If the shared connection was not locked by the thread, returns 1.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#sem  are NULL}
@end
@note Inherits errors from sem_op().
**/
int shcon_unlock_sem (shcon_t* _shcon);
/**
@brief Marks a connection on a semaphore.
@details The action will be undone when the thread disconnects.
@param _shcon The shared connection with the semaphore.
@return Upon success, returns 0.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#sem  are NULL}
@end
@note Inherits errors from sem_op().
**/
int shcon_add_sem_con (shcon_t* _shcon);
/**
@brief Starts a shared connection.
@details Assumes that \b _shcon shcon_t#sem and shcon_t#shm are set, but not
their sem_t#id and shm_t#id. Also assumes that the shcon_t#ipc is fully set.
The function will attempt to create a new semaphore and new shared memory.
If the semaphore and shared memory exist, it connects to them instead.
If one exist and the other does not, it initalizes one and recreates the other.
@param _shcon The shared connection to start.
@return Upon success, returns 0 and sets \b _shcon  shcon_t#sem and shcon_t#shm
sem_t#id and shm_t#id.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@note Resolves blame from shcon_create_sem_shm(), shcon_attach_sem_shm().
Inherits errors from shcon_create_sem_shm(), shcon_attach_sem_shm(),
shcon_add_sem_con().
@see @ref shcon_connect_protocol
**/
int shcon_connect (shcon_t* _shcon);
