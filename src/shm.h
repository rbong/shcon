#ifndef MM_SHM
#define MM_SHM
enum shm_flags_enum {
    SHM_USR_R = 0400,
    SHM_USR_W = 0200,
    SHM_GRP_R =  040,
    SHM_GRP_W =  020,
    SHM_OTH_R =   04,
    SHM_OTH_W =   02,
    SHM_CREAT = IPC_CREAT,
    SHM_EXCL  = IPC_EXCL,
};

typedef struct shm_t {
    char*  path;
    int    flags;
    int    id;
    int8_t proj_id;
    key_t  key;
} shm_t;

void  shm_generate_key_ftok (shm_t*);
void  shm_t_new             (shm_t*);
int   shm_assign_path       (shm_t*, char*, char*);

void (*shm_generate_key_func) (shm_t*) = shm_generate_key_ftok;

int shm_flags_def =  SHM_USR_R | SHM_USR_W | SHM_GRP_R | SHM_GRP_W | IPC_CREAT;

// move me to a global file
int8_t proj_id_def     = 'M';
char*  shm_root        = "/tmp/";
#endif
