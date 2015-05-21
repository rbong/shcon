// todo- add comments for function operation
#include <stdio.h>

#include <err.h>
#include <shm.h>

int main (int argc, char** argv)
{
    int    ret = 0;
    shm_t* shm = NULL;

    ret = shm_t_set_from_path (&shm, NULL, "test");

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

    shm_t_del (&shm);

    return 0;
}

