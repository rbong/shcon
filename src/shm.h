/** @file shm.h
@author Roger Bongers
@date May 28 2015
@brief Shared memory operations.
@see ipc.h
**/

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <ipc.h>
#include <err.h>
#include <str.h>
#include <file.h>
#include <msg.h>

/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_SHM
#define MM_SHM
/**
@brief Shared memory.
**/
typedef struct
{
    //! Common IPC data.
    ipc_t* ipc;
    //! The size in bytes.
    size_t size;
    //! The identifier of the shared memory. 
    int    id;
    //! The memory segment.
    void*  seg;
} shm_t;
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

/**
@brief Create a new shm_t.
@return Upon success, returns an address of a shm_t with empty members.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EALLOC, could not allocate space for the shm_t.}
@end
**/
shm_t* shm_t_new (void);
/**
@brief Populates a shm_t.
@param _shm The struct to populate.
@param _ipc If \b _ipc is not NULL, populates \b _shm with the ipc_t address.
If \b _ipc is NULL, does not populate \b _shm member \b ipc.
@param _size If \b _size is 0, populates \b _shm with #shm_size.
If \b _size is 1 or more, populates \b _shm with \b _size.
If \b _size is less than 0, does not populate \b _shm member \b size.
@param _id If \b _id is 0, generates a new id to populates \b _shm.
If \b _id is 1 or more, populates \b _shm with \b _id.
If \b _id is less than 0, does not populate \b _shm member \b id.
@param _seg If \b _seg is not NULL, populates \b _shm with the address.
If \b _seg is NULL, does not populate \b _shm member \b seg.
@return Upon success, returns 0 and populates \b _shm.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shm is NULL.}
@end
@note Inherits errors from shm_t_new(), shm_gen_id().
**/
int shm_t_set (shm_t** _shm, ipc_t* _ipc, size_t _size, int _id, void* _seg);
/**
@brief Populates a shm_t given IPC data.
@param _shm The shared memory to populate.
@param _ipc The IPC data.
@return Upon success, returns 0 and fully populates \b _shm.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, _ipc was NULL.}
@end
@note Inherits errors from shm_t_set(), shm_attach_seg()
**/
int shm_t_from_ipc (shm_t** _shm, ipc_t* _ipc);
/**
@brief Populates a shm_t given path information.
@param _shm The shared memory to populate.
@param _root,_sub Parameters to pass to ipc_gen_path().
@return Upon success, returns 0 and fully populates \b _shm.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@note Inherits errors from ipc_t_from_path,() shm_t_from_ipc()
**/
int shm_t_from_path (shm_t** _shm, char* _root, char* _sub);
/**
@brief Deletes a shm_t.
@details Assumes that \b _shm has been properly created by shm_t_new().
Does nothing if \b _shm or \b *_shm is NULL.
@returns Sets \b *_shm to NULL after freeing data.
@param _shm The shared memory to free.
@return Sets \b *_shm to NULL after freeing data.
**/
void shm_t_del (shm_t** _shm);
/**
@brief Generates a shm_t \b id.
@details If the shared memory already exists, does nothing after generating ID.
If it does not, it initializes the shared memory.
@param _shm The shm_t to generate the ID with.
@return Upon success, returns 0 and sets \b _shm \b id.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shm or \b _shm member \b ipc is NULL.}
@ent{_ESYSTEM, Failure in shared memory operations.}
@end
**/
int shm_gen_id (shm_t* _shm);
/**
@brief Attaches to a shared memory segment.
@param _shm The shared memory to retrieve the correct segment with.
@return Upon success, returns 0 and sets \b _shm \b seg.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shm is NULL.}
@ent{_ESYSTEM, Failure in shm operations.}
@end
**/
int shm_attach_seg (shm_t* _shm);
/**
@brief Reads from shared memory.
@param _shm The shared memory to read from.
@param _buf The memory to write to.
@param _nbytes The number of bytes to read.
@return Upon success, returns 0 and fills \b buf with the specified bytes.
<br> Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shm, \b _buf, or \b _shm member \b seg is NULL.}
@ent{_ESYSTEM, Error copying memory.}
@end
**/
int shm_read (shm_t* _shm, void* _buf, int _bytes);
/**
@brief Writes to shared memory.
@param _shm The shared memory to write to.
@param _buf The memory to read from.
@param _nbytes The number of bytes to write to.
@return Upon success, returns 0 and fills shared memory with the buffer.
<br> Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shm, \b _buf, or \b _shm member \b seg is NULL.}
@ent{_ESYSTEM, Error copying memory.}
@end
**/
int shm_write (shm_t* _shm, void* _buf, int _bytes);
