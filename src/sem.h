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
    //! The number of semaphores in the set.
    int    len;
    //! Semaphore set identifier.
    int    id;
} sem_t;

//! Type of the optional parameter for \b semctl() as specified by POSIX.
union sem_un
{
    int              val;
    struct semid_ds* buf;
    unsigned short*  array;
};
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

/**
@brief Create a new sem_t.
@return Upon success, returns the address of a sem_t with empty members.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EALLOC, could not allocate space for the sem_t.}
@end
**/
sem_t* sem_t_new (void);
/**
@brief Populates a sem_t.
@param _sem The semaphore to populate.
@param _len If \b _len is 0, populates \b _sem with #sem_len.
If \b _len is 1 or more, populates \b _sem with \b _len.
If \b _len is less than 0, does not populate \b _sem member \b len.
@param _id If \b _id is 0 and \b _key  and \b flags are valid,
generates a semaphore ID for \b _sem member \b id.
If \b _id is 1 or more, populates _sem with \b _id.
If \b _id is less than 0, does not populate \b _sem member \b id.
@param _key,flags Are used to set \b _sem \b id.
@return Upon success, returns 0 and populates \b _sem.
<br>If the population fails because of recoverable errors from sem_gen_id(),
returns 1 and passes the blame to the caller.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _sem is NULL.}
@ent{_SYSTEM, failure generating \b id.}
@end
@note Inherits errors from sem_t_new(), sem_gen_id(). Inherits blame from
sem_gen_id().
**/
int sem_t_set (sem_t** _sem, int _len, int _id, key_t _key, int _flags);
/**
@brief Deletes a sem_t.
@details Assumes that \b _sem has been properly created by sem_t_new().
Does nothing if \b _sem or \b *_sem is NULL.
@param _sem The semaphore to free.
@return Sets \b *_sem to NULL after freeing data.
**/
void sem_t_del (sem_t** _sem);
/**
@brief Generates a sem_t \b id.
@param _sem The sem_t to generate the ID for.
@param _key The unique key used to generate the correct ID.
@param _flags The permission flags of the semaphore.
@return Upon success, returns 0 and sets \b _sem \b id.
<br>If the generation fails because the semaphore already exists,
returns 1 without printing and does not set err_num.
<br>Upon failure, returns -1, sets #err_num and passes the blame to the caller.
@beg{Errors}
@ent{_EPTRNULL, \b _sem is NULL.}
@ent{_ESYSTEM, Failure in semaphore operations.}
@end
**/
int sem_gen_id (sem_t* _sem, key_t _key, int _flags);
/**
@brief Performs semaphore control operations.
@param _sem The semaphore to perform the operations on.
@param _sem_num,_cmd,_sem_un The values to pass to \b semctl() (system).
@return Upon success, returns 0 and performs the operations.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _sem is NULL.}
@ent{_ESYSTEM, Failure calling \b semctl().}
@end
**/
int sem_ctl (sem_t* _sem, int _sem_num, int _cmd, union sem_un _sem_un);
/**
@brief Performs semaphore operations.
@param _sem The semaphore to perform the operations on.
@param _sem_ops,_num_sem_ops The values to pass to \b semop() (system).
@return Upon success, returns 0 and performs the operations.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _sem is NULL.}
@ent{_ESYSTEM, Failure calling \b semop().}
@end
**/
int sem_op (sem_t* _sem, struct sembuf _sem_ops, size_t _num_sem_ops);
