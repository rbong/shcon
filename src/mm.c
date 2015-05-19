// todo- add comments for function operation
#include <stdio.h>

#include <shm.h>

int main (int argc, char** argv)
{
    shm_t* shm;

    shm_t_new (&shm, shm_root, "test", shm_proj_id_def,
               shm_flags_def, shm_generate_key_ftok);

    shm_t_del (&shm);

    return 0;
}

