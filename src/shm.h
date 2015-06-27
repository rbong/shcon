struct shm_t
{
    //! The size in bytes.
    size_t size;
    //! The identifier of the shared memory. 
    int    id;
    //! The memory segment.
    void*  seg;
};
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

typedef struct shm_t shm_t;

shm_t* shm_t_new (void);
int shm_t_from_id (shm_t** _shm, key_t _key, int _flags);
int shm_gen_id (shm_t* _shm, key_t _key, int _flags);
int shm_read (shm_t* _shm, void* _buf, int _bytes, int _offset);
int shm_write (shm_t* _shm, void* _buf, int _bytes, int _offset);
