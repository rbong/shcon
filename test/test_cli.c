#include <stdio.h>
#include <unistd.h>

#include <shm.h>
#include <err.h>

int main (int argc, char** argv)
{
    int ret = 0;
    char* s = NULL;
    shm_t* shm = shm_t_new ();


    shm_t_from_path (&shm, NULL, argv [1]);

    if (argv [2] [0] == 'w')
    {
        s = malloc (100);
        do
        {
            if (fgets (s, 100, stdin) == NULL)
            {
                ret = -1;
                ERR_AT_LINE_SYS (0, errno);
                err_set (_ESYSTEM);
                break;
            }

            shm_write (shm, &s, sizeof (char *));

            if (ret < 0)
            {
                break;
            }
        } while (1);
    }
    else if (argv [2] [0] == 'r')
    {
        do
        {
            shm_read (shm, &s, 0, sizeof (char*));
            if (s != NULL && *s != '\0')
            {
                printf ("%s", s);
                s [0] = '\0';
            }
            sleep (1);
        } while (1);
    }

    if (ret < 0)
    {
        ERR_PRINT (0);
        return 1;
    }

    return 0;
}
