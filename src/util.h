/** @file util.h
@author Roger Bongers
@date June 2 2015
@brief Helper utilities.
**/
/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_UTIL
#define MM_UTIL
#include <time.h>

#include <err.h>
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

/**
@brief Get current seconds.
@details Assumes that no one would want to timetravel to an age before Unix.
@return Upon success, returns time in seconds since the Epoch.
<br>Upon failure, returns -1.
@beg{Errors}
@ent{_ESYSTEM, Failure retrieving time.}
@end
**/
long get_sec (void);
