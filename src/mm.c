// todo- add comments for function operation
#include <mm.h>

int main (int argc, char** argv)
{
    shm_t shm;

    shm_t_new (&shm);

    shm_assign_path (&shm, NULL, "mysub");

    // todo- delete me
    printf ("%s\n", shm.path);

    shm_generate_key_func (&shm);
/*    check_error ((shm.key == -1), "main (): shm_generate_key_func ()"
                 EXIT_ERROR | USE_ERRNO);*/

    // todo- segsize is temporary value
    // shm_id = shmget (shm_key, segsize, 
}

