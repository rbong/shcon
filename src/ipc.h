
/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_IPC
#define MM_IPC
#include <stdlib.h>
#include <sys/ipc.h>

#include <err.h>
#include <str.h>
#include <file.h>

enum IPC_FLAGS {
    IPC_USR_R = 0400,
    IPC_USR_W = 0200,
    IPC_GRP_R =  040,
    IPC_GRP_W =  020,
    IPC_OTH_R =   04,
    IPC_OTH_W =   02,
};

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

extern key_t (*ipc_gen_key) (char* _path, int _proj_id);
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

typedef struct ipc_t ipc_t;

ipc_t* ipc_t_new (void);
int ipc_t_set (ipc_t** _ipc, int _flags, char* _path, key_t _key);
int ipc_t_from_path (ipc_t** _ipc, char* _path);
void ipc_t_del (ipc_t** _ipc);
key_t ipc_gen_key_ftok (char* _path, int _proj_id);
