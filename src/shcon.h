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
#ifndef SHCON_H
#define SHCON_H
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
If \b _ipc is NULL, does not populate \b _shcon member \b ipc.
@param _shm If \b _shm is not NULL, populates \b _shcon with the shm address.
If \b _shm is NULL, does not populate \b _shcon member \b shm.
@param _sem If \b _sem is not NULL, populates \b _shcon with the sem_t address.
If \b _sem is NULL, does not populate \b _shcon member \b sem.
@return Upon success, returns 0 and populates \b _shcon.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _sem is NULL.}
@end
@note Inherits errors from sem_t_new(), sem_gen_id().
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
