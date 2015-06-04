/** @file shcon_util.h
@author Roger Bongers
@date June 2 2015
@brief Shared connection internal utilities.
**/

#include <shcon.h>

/**
@brief Create a new semaphore.
@details Assumes \b _shcon member \b ipc member \b key and member \b flags
have been set.
@param _shcon The shared connection with the sem_t to generate an \b id for.
@return Upon success, returns 0 and sets \b _shcon member \b sem member \b id.
<br>Upon failure, returns -1 and passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon->ipc and/or \b _shcon->sem are NULL.}
@end
@note Inherits errors and blame from sem_gen_id().
**/
int shcon_create_sem_id (shcon_t* _shcon);
/**
@brief Attaches to an existing semaphore.
@details Assumes \b _shcon member \b ipc member \b key and member \b flags
have been set.
@param _shcon The shared connection with the sem_t to generate an \b id for.
@return Upon success, returns 0 and sets \b _shcon member \b sem member \b id.
<br>Upon failure, returns -1 and passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon->ipc and/or \b _shcon->sem are NULL.}
@end
@note Inherits errors and blame from sem_gen_id().
**/
int shcon_attach_sem_id (shcon_t* _shcon);
/**
@brief Sets pertinent values for a new semaphore in the system.
@param _shcon The shared connection with the sem_t to set values for.
@return Upon success, returns 0.
<br>Upon failure, returns -1 and possibly passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon->sem are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits errors from sem_ctl().
**/
int shcon_init_sem (shcon_t* _shcon);
/**
@brief Create new shared memory.
@details Assumes \b _shcon member \b ipc members \b key \b flags have been set.
@param _shcon The shared connection with the shm_t to generate an \b id for.
@return Upon success, returns 0 and sets \b _shcon member \b shm member \b id.
<br>Upon failure, returns -1 and passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon->ipc and/or \b _shcon->shm are NULL.}
@end
@note Inherits errors and blame from shm_gen_id().
**/
int shcon_create_shm_id (shcon_t* _shcon);
/**
@brief Attaches to existing shared memory.
@details Assumes \b _shcon member \b ipc member \b key and member \b flags
have been set.
@param _shcon The shared connection with the shm_t to generate an \b id for.
@return Upon success, returns 0 and sets \b _shcon member \b shm member \b id.
<br>Upon failure, returns -1 and passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon->ipc and/or \b _shcon->shm are NULL.}
@end
@note Inherits errors and blame from shm_gen_id().
**/
int shcon_attach_shm_id (shcon_t* _shcon);
/**
@brief Sends an init message over shared memory.
@details Assumes that program is in the init phase and does not need to lock.
@param _shcon The shared connection with the shm_t to send the message over.
@return Upon success, returns 0.
<br>Upon failure, returns -1 and possibly passes the blame to the caller.
@beg {Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon->shm are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits errors from shcon_send_shm_msg.
**/
int shcon_init_shm (shcon_t* _shcon);
/**
@brief Sends a disconnect message over shared memory.
@details Assumes that program is in the init phase and does not need to lock.
@param _shcon The shared connection with the shm_t to send the message over.
@return Upon success, returns 0.
<br>Upon failure, returns -1 and possibly passes the blame to the caller.
@beg {Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon->shm are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits errors from shcon_send_shm_msg.
**/
int shcon_kill_shm (shcon_t* _shcon);
/**
@brief Checks that the shared connection has the correct version.
@details Assumes that shcon_t \b shm is set and has an init message.
Locks the thread until the shared connection is available.
@param _shcon The shared connection to check the version of.
@return Upon success, returns 0 if the shared connection has the right version.
<br>Upon failure or a bad version,
returns -1 and possibly passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon was NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits errors from shcon_lock_sem(), shcon_recv_shm_msg(),
shcon_unloc_sem()
@end
**/
int shcon_check_shm_ver (shcon_t* _shcon);
/**
@brief Create shared memory and a semaphore inside of a shared connection.
@details May connect to existing shared memory if semaphore did not exist.
@param _shcon The shared connection to create the structs for.
@return Upon success,
returns 0 and populates \b _shcon members \b shm and \b sem members \b id.
<br>Upon failure, returns -1 and possibly passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon->sem and/or \b _shcon->shm are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@see protocol
@note Inherits errors from shcon_create_sem_id(), shcon_create_kill_shm(),
shcon_init_sem(), and shcon_unlock_sem().
**/
int shcon_create_sem_shm (shcon_t* _shcon);
/**
@brief Create new shared memory, or attach and send disconnect message.
@details Assumes that program is in the init phase and does not need to lock.
@param _shcon The shared connection with the shm_t to alter.
@return Upon success, returns 0 and sets \b _shcon member \b shm member \b id.
<br>Upon failure, returns -1 and possibly passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon->shm are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits errors from shcon_create_shm_id(), shcon_attach_shm_id(),
shcon_kill_shm(), and shcon_init_shm().
**/
int shcon_create_kill_shm (shcon_t* _shcon);
/**
@brief Attach to shared memory and a semaphore inside of a shared connection.
@details If the sem_t \b id is not 0, does not attempt to attach semaphore.
@param _shcon The shared connection to attach the structs for.
@return Upon success,
returns 0 and populates \b _shcon members \b shm and \b sem members \b id.
<br>Upon failure, returns -1 and possibly passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon->sem and/or \b _shcon->shm are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@see protocol
@note Inherits errors from shcon_attach_sem_id(), shcon_attach_shm_id(),
shcon_check_shm_ver().
**/
int shcon_attach_sem_shm (shcon_t* _shcon);
