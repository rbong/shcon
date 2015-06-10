/** @file shcon_intern.h
@author Roger Bongers
@date June 2 2015
@brief Shared connection internals.
**/

/** @fn _shcon_create_sem()
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
/** @fn _shcon_attach_sem()
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
/** @fn _shcon_init_sem()
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
/** @fn _shcon_create_shm()
@brief Create new shared memory.
@details Assumes \b _shcon shcon_t#ipc ipc_t#key and ipc_t#flags have been set
and that \b _shcon shcon_t#shm is set except for its shm_t#id and shm_t#seg.
@param _shcon The shared connection with the shcon_t#shm to generate a
shm_t#id and shm_t#seg for.
@return Upon success, returns 0 and sets \b _shcon shcon_t#shm shm_t#id and
shm_t#seg.
<br>Upon failure, returns -1 and passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon shcon_t#ipc and/or
\b _shcon shcon_t#shm are NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits blame from shm_gen_id().
Inherits errors from shm_gen_id(), shm_attach_seg().
**/
/** @fn _shcon_attach_shm()
@brief Attaches to existing shared memory.
@details Assumes \b _shcon shcon_t#ipc ipc_t#key and ipc_t#flags have been set
and that \b _shcon shcon_t#shm is set except for its shm_t#id and shm_t#seg.
@param _shcon The shared connection with the shcon_t#shm to generate a shm_t#id
and shm_t#seg for.
@return Upon success, returns 0 and sets \b _shcon shcon_t#shm shm_t#id and
shm_t#seg.
<br>Upon failure, returns -1 and passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon\, \b _shcon shcon_t#ipc and/or
\b _shcon shcon_t#shm are NULL.}
@end
@note Inherits blame from shm_gen_id().
Inherits errors from shm_gen_id(), shm_attach_seg().
**/
/** @fn _shcon_init_shm()
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
/** @fn _shcon_check_shm_ver()
@brief Checks that the shared connection has the correct version.
@details Assumes that shcon_t#shm shcon_t#shm is set and has an init message.
Does not lock the thread.
@param _shcon The shared connection to check the version of.
@return Upon success, returns 0 if the shared connection has the right version.
<br>Upon failure or a bad version,
returns -1 and possibly passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _shcon was NULL.}
@ent{_ESUCCESS, blame will not be passed to the caller.}
@end
@note Inherits errors from shcon_recv_shm_msg(),
shcon_unloc_sem()
@end
**/
