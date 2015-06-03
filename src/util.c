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

    tmp = time (NULL);
    if (tmp < 0)
    {
        ERR_PRINT (_ESYSTEM);
    }
    ret = tmp;
    return ret;
}
