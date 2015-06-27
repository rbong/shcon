#include <mm.h>

// example client
int f (void* _con, msg_t** _msg, int _flag)
{
    int tmp = 0;
    int ret = 0;
    long _type = 0;
    char _s [100];
    static int last = 0;

    static msg_t* _push_msg = NULL;

    if (_msg == NULL)
    {
        ERR_PRINT (_EPTRNULL);
        ret = -1;
        return ret;
    }

    switch (_flag)
    {
        case ANON_SEND:
            if ((*_msg == NULL) || (*_msg)->data == NULL)
            {
                ERR_PRINT (_EPTRNULL);
                ret = -1;
                return ret;
            }
            printf ("%s\n", (*_msg)->data);
            break;
        case ANON_RECV:
            if (_push_msg != NULL)
            {
                *_msg = _push_msg;
                _push_msg = NULL;
            }
            else
            {
                printf ("Enter message: \n");
                if (fgets (_s, 100, stdin) == NULL)
                {
                    ERR_FROM ();
                    ret = -1;
                    return ret;
                }

                if (_s != NULL && strcmp (_s, "\n") == 0)
                {
                    ret = 0;
                    return ret;
                }

                if (_s != NULL && strcmp (_s, "k\n") == 0)
                {
                    _type = MSG_KILL;
                }

                *_msg = msg_t_new ();
                if (*_msg == NULL)
                {
                    ERR_FROM ();
                    ret = -1;
                    return ret;
                }

                tmp = msg_t_set (_msg, _type, (msg_hdr_t) { 0 }, _s);
                if (tmp < 0)
                {
                    ERR_FROM ();
                    ret = tmp;
                    return ret;
                }
                (*_msg)->hdr.timeout = 1000;
            }
            ret = 1;
            break;
        case ANON_PUSH:
            if ((*_msg == NULL))
            {
                ERR_PRINT (_EPTRNULL);
                ret = -1;
                return ret;
            }
            // todo- copy data
            _push_msg = (*_msg);
            _push_msg ->data = (*_msg)->data;
            (*_msg)->data = NULL;
            break;
        default:
            ERR_PRINT (_ENOFLAG);
            ret = -1;
            break;
    }
    return ret;
}
int main (int argc, char** argv)
{
    int tmp = 0;
    int ret = 0;
    int _attach = 0;
    ipc_t* _ipc = NULL;
    shcon_t* _shcon = NULL;

    if (argc != 2)
    {
        fprintf (stderr, "Please enter just a file name.\n");
        ret = 1;
        return ret;
    }

    tmp = ipc_t_from_path (&_ipc, argv [1]);
    if (tmp < 0)
    {
        ERR_FROM ();
        ret = tmp;
        return ret;
    }

    tmp = shcon_t_set (&_shcon, _ipc, NULL, NULL);
    if (tmp < 0)
    {
        if (err_num != _ENOBLAME)
        {
            if (errno != 0)
            {
                ERR_SYS (errno);
            }
            ERR_PRINT (err_num);
        }
        ret = tmp;
        return ret;
    }

    tmp = _attach = shcon_connect (_shcon);
    if (tmp < 0)
    {
        ERR_FROM ();
        ret = tmp;
        shcon_unlock_sem (_shcon);
        return ret;
    }

    tmp = shcon_msg_loop (_shcon, _attach, NULL, f);
    if (tmp < 0)
    {
        ret = 1;
        ERR_FROM ();
    }
    shcon_unlock_sem (_shcon);
    return ret;
}
