#include <stdio.h>
#include <unistd.h>

#include <shm.h>
#include <err.h>

int main (int argc, char** argv)
{
    int    ret = 0;
    shm_t* shm = NULL;
    char   buf [100];

    ret = shm_t_from_path (&shm, NULL, "test");

    if (ret < 0)
    {
        ERR_PRINT (0);
        return 1;
    }

    if (argv [1] [0] == 'w')
    {
        do
        {
            if (fgets (buf, 100, stdin) == NULL)
            {
                ret = -1;
                ERR_AT_LINE_SYS (0, errno);
                err_set (_ESYSTEM);
                break;
            }

            ret = shm_write (shm, buf, 100);

            if (ret < 0)
            {
                break;
            }
        } while (buf != NULL && buf [0] != '\n');
    }
    else if (argv [1] [0] == 'r')
    {
        do
        {
            ret = shm_read (shm, buf, 100);
            if (buf [0] != '\0')
            {
                printf ("%s", buf);
                shm_write (shm, "\0", 1);
            }
            sleep (1);
        } while (buf != NULL && buf [0] != '\n');
    }

    if (ret < 0)
    {
        ERR_PRINT (0);
        return 1;
    }

    shm_t_del (&shm);

    return 0;
}
