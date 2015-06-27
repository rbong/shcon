/** @file shcon.h
@author Roger Bongers
@date May 28 2015
@brief Shared connection operations.
@section shcon_connect_protocol Connection protocol
The following is an illustration of the connection protocol.
Fatal errors are not included.
Click the boxes to view the corresponding functions and protocols.
@dotfile shcon_connect.dot
@see shm.h sem.h ipc.h msg.h
@section shcon_msg_protocol Message loop
Fatal errors are not included.
Click the boxes to view the corresponding functions and protocols.
@dotfile shcon_msg_loop.dot
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
    //! The semaphore set to indicate locking.
    SEMSET_LOCK = 0,
    //! The semaphore set to indicate connections. Automatically decrements.
    SEMSET_CON  = 1,
    //! The semaphore set to indicate unique readings of the message.
    SEMSET_READ = 2,
};

/**
@brief A shared connection.
**/
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

/**
@brief Create a new shcon_t.
@return Upon success, returns an address of a shcon_t with empty members.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EALLOC, Could not allocate space for the shcon_t.}
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
int shcon_t_from_path (shcon_t** _shcon, char* _path);
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
@details Assumes the thread has been locked.
@param _shcon The shared connection with the shared memory.
@param _msg The message to send.
@return Upon success, returns 0.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#shm are NULL}
@end
@note Inherits errors from msg_to_raw(), shcon_lock_sem(), shm_write(),
shcon_unlock_sem().
**/
int shcon_send_shm_msg (shcon_t* _shcon, msg_t* _msg);
int shcon_send_empty_shm_msg (shcon_t* _shcon);
/**
REWRITE!!
@brief Gets a message from shared memory.
@details Assumes the thread has been locked.
@param _shcon The shared connection with the shared memory.
@param _init Indicates to read the header message instead of post-header.
@return Upon success, returns a newly allocated message.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b l_shcon shcon_t#shm are NULL}
@end
@note Inherits errors from msg_t_new(), msg_to_raw_len(), shm_read().
**/
int shcon_recv_shm_msg (shcon_t* _shcon, msg_t** _msg, int _init);
/**
@brief Marks the current message as read on a semaphore.
@details The action will be undone when the thread disconnects.
@param _shcon The shared connection with the semaphore.
@return Upon success, returns 0.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#shm are NULL}
@end
@note Inherits errors from sem_op().
**/
int shcon_mark_sem_read (shcon_t* _shcon);
/**
@brief Locks a semapohre.
@details Locks the thread until the shared connection is available.
@param _shcon The shared connection with the semaphore to lock.
@return Upon success, returns 0 and sets \b _shcon shcon_t#locked to 1.
<br>If the shared connection was already locked by the thread, returns 1.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#sem are NULL}
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
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#sem are NULL}
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
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#sem are NULL}
@end
@note Inherits errors from sem_op().
**/
int shcon_add_sem_con (shcon_t* _shcon);
int shcon_get_sem_con (shcon_t* _shcon);
int shcon_get_sem_read (shcon_t* _shcon);
/**
@brief Starts a shared connection.
@details This function follows the protocol in @ref shcon_connect_protocol.
Assumes that \b _shcon shcon_t#sem and shcon_t#shm are set, but not
their sem_t#id shm_t#id and shm_t#seg. Also assumes that the shcon_t#ipc is
fully set. The function will attempt to create or connect to a new semaphore
and shared memory segment.
If the semaphore is new and the shared memory is not, it assumes that the
previously attached threads were aware of the malformation and have
disconnected.
The function keeps the semaphore locked to perform any desired operations
before allowing other threads to write to the shared memory.
@param _shcon The shared connection to start.
@return Upon success, returns 0 if the shared memory was attached to or 1 if it
was created.
Sets \b _shcon shcon_t#sem sem_t#id and shcon_t#shm shm_t#id and shm_t#seg,
locks the shcon_t#sem, and adds to its #SEMSET_READ.
<br>Upon failure, returns -1, prints errors if necessary, sets #err_num, and
attempts to unlock \b _shcon shcon_t#sem.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#sem are NULL}
@ent{_EBADVAL, \b _shcon shcon_t#shm shm_t#id is already set.}
@end
@note Resolves blame from _shcon_create_sem(), _shcon_attach_sem(),
_shcon_create_shm(), shcon_attach_shm().
Inherits errors from _shcon_create_sem(), _shcon_attach_sem(), shcon_lock_sem(),
shcon_unlock_sem(), _shcon_init_sem(), shcon_add_sem_con(), shcon_attach_shm(),
_shcon_create_shm(), _shcon_create_shm(), shcon_check_shm_ver().
@see @ref shcon_connect_protocol
**/
int shcon_connect (shcon_t* _shcon);
int shcon_msg_loop
  (shcon_t* _shcon, int _create, void* _con, int (*_f) (void*, msg_t**, int));
