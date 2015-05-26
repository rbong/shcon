// todo- check consistency among parallel structure functions
// todo- write functions that test if structs are empty
// todo- name structs with different names for the type and structure
// todo- check that complex return statements are correct
// todo- rename res to tmp and add it everywhere
// todo- remove newlines from related statements
// todo- add comments for function operation and file purposes
// todo- rehaul error framework
// todo- return implicitly from void functions
// todo- write test functions
#include <stdio.h>
#include <unistd.h>

#include <err.h>
#include <shm.h>

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
