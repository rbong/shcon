/** @file sem.h
@author Roger Bongers
@date May 28 2015
@brief Semaphore processing.
@see shcon.h sem_add_con()
**/

#include <stdlib.h>
#include <sys/sem.h>

#include <err.h>

/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_SEM
#define MM_SEM
/**
@brief A set of semaphores.
**/
typedef struct
{
    //! The constant number of semaphores in the set, as specified by #sem_len.
    int    len;
    //! The identifier of the set.
    int    id;
    //! Specifies whether this process has locked the semaphore.
    int    locked;
} sem_t;

//! Type of the optional parameter for \b semctl() as specified by POSIX.
union semun
{
    int              val;
    struct semid_ds* buf;
    unsigned short*  array;
};

//! The number of semaphores in each sem_t.
extern int sem_len;
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

/**
@brief Create a new sem_t.
@return Upon success, returns an address of a sem_t with empty members.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EALLOC, could not allocate space for the sem_t.}
@end
**/
sem_t* sem_t_new (void);
/**
@brief Populates a sem_t.
@param _sem The semaphore to populate.
@param _id If \b _id is 0, \b _key is more than 0, and \b flags is 0 or more,
generates a semaphore ID to populates \b _sem.
If \b _id is 1 or more, populates _sem with \b _id.
If \b _id is less than 0, does not populate \b _sem member \b id.
@param _key,flags Are used to set \b _sem \b id.
@return Upon success, returns 0 and populates \b _sem.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _sem is NULL.}
@end
@note Inherits errors from sem_t_new(), sem_gen_id().
**/
int sem_t_set (sem_t** _sem, int _id, key_t _key, int _flags);
/**
@brief Deletes a sem_t.
@details Assumes that \b _sem has been properly created by sem_t_new().
Does nothing if \b _sem or \b *_sem is NULL.
@returns Sets \b *_sem to NULL after freeing data.
@param _sem The semaphore to free.
@return Sets \b *_sem to NULL after freeing data.
**/
void sem_t_del (sem_t** _sem);
/**
@brief Generates a sem_t \b id.
@details If the semaphore set already exists, does nothing after generating ID.
If it does not, it initializes the semaphore set.
@param _sem The sem_t to generate the ID for.
@param _key The unique key used to generate the correct ID.
@param _flags The permission flags of the semaphore.
@return Upon success, returns 0 and sets \b _sem \b id.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _sem is NULL.}
@ent{_ESYSTEM, Failure in semaphore operations.}
@end
@note Inherits errors from sem_add_con()
@see sem_add_con()
**/
int sem_gen_id (sem_t* _sem, key_t _key, int _flags);
/**
@brief Locks the semaphore.
@details This function holds the thread until the semaphore is not locked.
@param _sem The semaphore to lock.
@return Upon success, returns 0.
<br>If the semaphore was already locked by this thread, returns 1.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _sem is NULL.}
@ent{_ESYSTEM, Unable to lock semaphore.}
@end
**/
int sem_lock (sem_t* _sem);
/**
@brief Unlocks the semaphore.
@details This function does not wait for the semaphore to become available.
@param _sem The semaphore to unlock.
@return Upon success, returns 0.
<br>If the semaphore was not locked by this thread, returns 1.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _sem is NULL.}
@ent{_ESYSTEM, Unable to unlock semaphore.}
@end
**/
int sem_unlock (sem_t* _sem);
