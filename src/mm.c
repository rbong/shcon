// todo- add comments for function operation
#include <stdio.h>

#include <err.h>
#include <shm.h>

int main (int argc, char** argv)
{
    int    ret = 0;
    shm_t* shm = NULL;
    char   buf [100];

    ret = shm_t_new (&shm, shm_root, "test", shm_proj_id_def,
                     shm_flags_def, shm_generate_key_ftok);

    if (ret < 0)
    {
        ERR_PRINT (0);
        return 1;
    }

    ret = shm_write (shm, "test1\ntest2\n", 100);

    if (ret < 0)
    {
        ERR_PRINT (0);
        return 1;
    }

    ret = shm_write (shm, "test3\ntest4\n", 100);

    if (ret < 0)
    {
        ERR_PRINT (0);
        return 1;
    }

    ret = shm_t_del (&shm);

    if (ret < 0)
    {
        ERR_PRINT (0);
        return 1;
    }

    return 0;
}

