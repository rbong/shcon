#include <stdio.h>
#include <unistd.h>

#include <shcon.h>
#include <err.h>

int main (int argc, char** argv)
{
    int      ret = 0;
    shcon_t* shcon = NULL;
    char     buf [100];

    ret = shcon_t_from_path (&shcon, NULL, "test");

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

            ret = shm_write (shcon->shm, buf, 100);

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
            ret = shm_read (shcon->shm, buf, 100);
            if (buf [0] != '\0')
            {
                printf ("%s", buf);
                shm_write (shcon->shm, "\0", 1);
            }
            sleep (1);
        } while (buf != NULL && buf [0] != '\n');
    }

    if (ret < 0)
    {
        ERR_PRINT (0);
        return 1;
    }

    shcon_t_del (shcon);

    return 0;
}
