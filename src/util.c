/** @file util.c
@author Roger Bongers
@date June 2 2015
@brief See util.h.
@see util.h
**/

#include <util.h>

long get_sec (void)
{
    time_t tmp = 0;
    long   ret = 0;
    
    static long _last = 0;

    tmp = time (NULL);
    ret = tmp;
    if (tmp < 0)
    {
        ERR_PRINT (_ESYSTEM);
        return ret;
    }

    if (_last != 0 && tmp < _last)
    {
        ERR_PRINT (_EBADTIME);
        ret = -1;
        return ret;
    }

    return ret;
}
