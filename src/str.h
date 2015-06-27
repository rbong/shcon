/**
@file str.h
@author Roger Bongers
@date May 28 2015
@brief String processing.
**/

/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_STR
#define MM_STR
#include <string.h>

#include <err.h>
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */
/**
@brief Combines strings.
@details if _strs is less than 1, does nothing.
@param _strs The number of strings.
@param _dest The destination address.
@param _src The strings to combine.
@return Upon success, returns 0 and fills \b _dest with the strings.
<br> Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _dest\, \b _src\, and/or a value in \b _src are NULL.}
@end
**/
int str_cat (int _strs, char* _dest, char** _src);
/**
@brief Calculates the size of combined strings.
@param _strs The number of strings.
@param _src The strings to combine.
@return Upon success, returns the size of the combined strings.
<br> Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _src and/or a value in \b _src is NULL.}
@end
**/
int str_cat_len (int _strs, char** _src);
