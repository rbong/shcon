/** @file shcon_util.h
@author Roger Bongers
@date June 2 2015
@brief Shared connection internal utilities.
**/

#include <shcon.h>

/**
@brief Create a new semaphore.
@details Assumes \b _shcon shcon_t#ipc ipc_t#key and ipc_t#flags have been set
and that \b _shcon shcon_t#sem is set except for its sem_t#id.
@param _shcon The shared connection with the shcon_t#sem to generate a
sem_t#id for.
@return Upon success, returns 0 and sets \b _shcon shcon_t#sem sem_t#id.
<br>Upon failure, returns -1 and passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon shcon_t#ipc and/or
\b _shcon shcon_t#sem are NULL.}
@end
@note Inherits errors and blame from sem_gen_id().
**/
int shcon_create_sem_id (shcon_t* _shcon);
/**
@brief Attaches to an existing semaphore.
@details Assumes \b _shcon shcon_t#ipc ipc_t#key and ipc_t#flags have been set
and that \b _shcon shcon_t#sem is set except for its sem_t#id.
@param _shcon The shared connection with the shcon_t#sem to generate a
sem_t#id for.
@return Upon success, returns 0 and sets \b _shcon shcon_t#sem sem_t#id.
<br>Upon failure, returns -1 and passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon shcon_t#ipc and/or
\b _shcon shcon_t#sem are NULL.}
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
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#sem are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits errors from sem_ctl().
**/
int shcon_init_sem (shcon_t* _shcon);
/**
@brief Create new shared memory.
@details Assumes \b _shcon shcon_t#ipc ipc_t#key and ipc_t#flags have been set
and that \b _shcon shcon_t#shm is set except for its shm_t#id and shm_t#seg.
@param _shcon The shared connection with the shcon_t#shm to generate a
shm_t#id for.
@return Upon success, returns 0 and sets \b _shcon shcon_t#shm sem_t#id.
<br>Upon failure, returns -1 and passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon shcon_t#ipc and/or
\b _shcon shcon_t#shm are NULL.}
@end
@note Inherits errors and blame from shm_gen_id().
**/
int shcon_create_shm_id (shcon_t* _shcon);
/**
@brief Attaches to existing shared memory.
@details Assumes \b _shcon shcon_t#ipc ipc_t#key and ipc_t#flags have been set
and that \b _shcon shcon_t#shm is set except for its shm_t#id and shm_t#seg.
@param _shcon The shared connection with the shcon_t#shm to generate an shm_t#id for.
@return Upon success, returns 0 and sets \b _shcon shcon_t#shm sem_t#id.
<br>Upon failure, returns -1 and passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon shcon_t#ipc and/or
\b _shcon shcon_t#shm are NULL.}
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
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#shm are NULL.}
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
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#shm are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits errors from shcon_send_shm_msg.
**/
int shcon_kill_shm (shcon_t* _shcon);
/**
@brief Checks that the shared connection has the correct version.
@details Assumes that shcon_t#shm shcon_t#shm is set and has an init message.
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
returns 0 and populates \b _shcon shcon_t#shm shm_t#id
and shcon_t#sem sem_t#id.
<br>Upon failure, returns -1 and possibly passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon shcon_t#sem and/or
\b _shcon shcon_t#shm are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits errors from shcon_create_sem_id(), shcon_create_kill_shm(),
shcon_init_sem(), and shcon_unlock_sem().
**/
int shcon_create_sem_shm (shcon_t* _shcon);
/**
@brief Create new shared memory, or attach and send disconnect message.
@details Assumes that program is in the init phase and does not need to lock.
@param _shcon The shared connection with the shm_t to alter.
@return Upon success, returns 0 and sets \b _shcon shcon_t#shm shm_t#id.
<br>Upon failure, returns -1 and possibly passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon and/or \b _shcon shcon_t#shm are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits errors from shcon_create_shm_id(), shcon_attach_shm_id(),
shcon_kill_shm(), and shcon_init_shm().
**/
int shcon_create_kill_shm (shcon_t* _shcon);
/**
@brief Attach to shared memory and a semaphore inside of a shared connection.
@details If the shcon_t#sem sem_t#id is not 0,
does not attempt to attach semaphore.
@param _shcon The shared connection to attach the structs for.
@return Upon success,
returns 0 and populates \b _shcon shcon_t#shm shm_t#id
and shcon_t#sem sem_t#id.
<br>Upon failure, returns -1 and possibly passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon shcon_t#sem and/or
\b _shcon shcon_t#shm are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits errors from shcon_attach_sem_id(), shcon_attach_shm_id(),
shcon_check_shm_ver().
**/
int shcon_attach_sem_shm (shcon_t* _shcon);
