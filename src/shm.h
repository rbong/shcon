/** @file shm.h
@author Roger Bongers
@date May 28 2015
@brief Shared memory operations.
**/

#include <stdlib.h>
#include <sys/shm.h>

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
@param _size If \b _size is 0, populates \b _shm with #shm_size.
If \b _size is 1 or more, populates \b _shm with \b _size.
If \b _size is less than 0, does not populate \b _shm member \b size.
@param _id If \b _id is 0 and \b _key  and \b flags are valid,
generates a shared memory ID for \b _shm member \b id.
If \b _id is less than 0, does not populate \b _shm member \b id.
@param _seg If \b _seg is not NULL, populates \b _shm with the address.
If \b _seg is NULL and \b _shm member \b id is valid,
\b _shm member \b seg is attached to the memory indicated by \b id.
If \b _seg is NULL and \b _shm member \b id is invalid,
does not populate \b _shm member \b seg.
@param _key,flags Are used to set \b _shm \b id.
@return Upon success, returns 0 and populates \b _shm.
<br>If the population fails because the shared memory already exists,
returns 1 without printing and does not set err_num.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shm is NULL.}
@end
@note Inherits errors from shm_t_new(), shm_gen_id(), shm_attach_seg().
**/
int shm_t_set
  (shm_t** _shm, size_t _size, int _id, void* _seg, key_t key, int _flags);
/**
@brief Populates a shm_t with information to generate an \b id.
@param _shm The struct to populate.
@param _key,flags Are used to set \b _shm \b id.
@param _key,flags Are used to set \b _shm \b id.
@return Upon success, returns 0 and populates \b _shm.
<br>If the population fails because the shared memory already exists,
returns 1 without printing and does not set err_num.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@note Inherits errors from shm_t_set()
**/
int shm_t_from_id (shm_t** _shm, key_t _key, int _flags);
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
@param _shm The shm_t to generate the ID with.
@param _key The unique key used to generate the correct ID.
@param _flags The permission flags of the shared memory.
@return Upon success, returns 0 and sets \b _shm \b id.
<br>If the generation fails because the shared memory already exists,
returns 1 without printing and does not set err_num.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shm is NULL.}
@ent{_ESYSTEM, Failure in shared memory operations.}
@end
**/
int shm_gen_id (shm_t* _shm, key_t _key, int _flags);
/**
@brief Attaches to a shared memory segment.
@param _shm The shared memory to retrieve the correct segment with.
@param _shmaddr,_shmflg Parameters to pass to system function \b shmat.
@return Upon success, returns 0 and sets \b _shm \b seg.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shm is NULL.}
@ent{_ESYSTEM, Failure in shm operations.}
@end
**/
int shm_attach_seg (shm_t* _shm, const void* _shmaddr, int _shmflg);
/**
@brief Reads from shared memory.
@param _shm The shared memory to read from.
@param _buf The memory to write to.
@param _bytes The number of bytes to read.
@param _offset The offset at which to read from the memory.
@return Upon success, returns 0 and fills \b buf with the specified bytes.
<br> Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shm, \b _buf, or \b _shm member \b seg is NULL.}
@ent{_ESYSTEM, Error copying memory.}
@ent{_EBADVAL, \b _offset was out of bounds.}
@end
**/
int shm_read (shm_t* _shm, void* _buf, int _bytes, int _offset);
/**
@brief Writes to shared memory.
@param _shm The shared memory to write to.
@param _buf The memory to read from.
@param _bytes The number of bytes to write to.
@param _offset The offset at which to write to the memory.
@return Upon success, returns 0 and fills shared memory with the buffer.
<br> Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _shm, \b _buf, or \b _shm member \b seg is NULL.}
@ent{_ESYSTEM, Error copying memory.}
@ent{_EBADVAL, \b _offset was out of bounds.}
@end
**/
int shm_write (shm_t* _shm, void* _buf, int _bytes, int _offset);
