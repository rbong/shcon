/** @file ipc.h
@author Roger Bongers
@date May 27 2015
@brief Generic inter-process communication processing.
@see sem.h shm.h
**/

/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_IPC
#define MM_IPC
#include <stdlib.h>
#include <sys/ipc.h>

#include <err.h>
#include <str.h>
#include <file.h>

/**
@brief The permission flags for IPC.
@details IPC_CREAT and IPC_EXCL are often used as IPC flags, but here they are
disabled by many functions that use an ipc_t and re-enabled when necessary.
**/
enum IPC_FLAGS {
    IPC_USR_R = 0400,
    IPC_USR_W = 0200,
    IPC_GRP_R =  040,
    IPC_GRP_W =  020,
    IPC_OTH_R =   04,
    IPC_OTH_W =   02,
};

/**
@brief Common IPC data.
**/
struct ipc_t
{
    //! The permission flags.
    int flags;
    //! An agreed upon character for key generation.
    int proj_id;
    //! An agreed upon path for key generation.
    char* path;
    //! A common key among processes.
    key_t key;
};

/**
@brief The function to use to generate keys.
@details ipc_gen_key_ftok() by default.
@param _path The name of the file to generate the key from.
@param _proj_id An agreed upon value used to generate a unique key.
@return Upon success, returns the key.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _path was NULL.}
@ent{_ESTREMPTY, \b _path begins with a '\0' character.}
@ent{_ESYSTEM, Error producing the key.}
@end
**/
extern key_t (*ipc_gen_key) (char* _path, int _proj_id);
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

typedef struct ipc_t ipc_t;

/**
@brief Create a new ipc_t.
@return Upon success, returns an address of an ipc_t with empty members.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EALLOC, Could not allocate space for the ipc_t.}
@end
**/
ipc_t* ipc_t_new (void);
/**
@brief Populates an ipc_t.
@param _ipc The struct to populate.
@param _flags If \b _flags is 0, populates \b _ipc with #ipc_flags.
If \b _flags is 1 or more, populates \b _ipc with \b _flags.
If \b _flags is less than 0, does not populate \b _ipc ipc_t#flags.
@param _path If \b _path is not NULL, populates \b _ipc with the address.
If \b _path is NULL, does not populate \b _ipc ipc_t#path.
@param _key If \b _key is 0 and \b _ipc \b path and \b _ipc ipc_t#proj_id are
valid, generates a new key to populate \b _ipc.
If \b _key is 1 or more, populates \b _ipc with \b _key.
If \b _key is less than 0, does not populate \b _ipc ipc_t#key.
@return Upon success, returns 0 and populates \b _ipc.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _ipc is NULL.}
@end
@note Inherits errors from ipc_t_new(), ipc_gen_key().
**/
int ipc_t_set (ipc_t** _ipc, int _flags, char* _path, key_t _key);
/**
@brief Populates an ipc_t given path information.
@param _ipc The struct to populate.
@param _path The string to populate ipc_t ipc_t#path with.
@return Upon success, returns 0 and populates \b _ipc.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _path is NULL.}
@end
@note Inherits errors from ipc_t_set().
**/
int ipc_t_from_path (ipc_t** _ipc, char* _path);
/**
@brief Deletes an ipc_t.
@details Assumes that \b _ipc has been properly created by ipc_t_new().
Does nothing if \b _ipc or \b *_ipc is NULL.
@returns Sets \b *_ipc to NULL after freeing data.
@param _ipc The struct to free.
@return Sets \b *_ipc to NULL after freeing data.
**/
void ipc_t_del (ipc_t** _ipc);
/**
@brief Generates keys with \b ftok()
@param _path The name of the file to generate the key from.
@param _proj_id An agreed upon value used to generate a unique key.
@return Upon success, returns the key.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _path is NULL.}
@ent{_ESTREMPTY, \b _path begins with a '\0' character.}
@ent{_ESYSTEM, Error producing the key with \b ftok().}
@end
**/
key_t ipc_gen_key_ftok (char* _path, int _proj_id);
