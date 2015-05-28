/**
@file file.h
@author Roger Bongers
@date May 27 2015
@brief File processing.
**/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <err.h>

/**
@brief Create an empty file if it does not exist.
@param _file_name The path to the file.
@return Upon success, returns 0 if the file did not exist and 1 if it did.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _file_name was NULL.}
@ent{_ESTREMPTY, \b _file_name begins with a '\0' character.}
@ent{_EFOPEN, Could not open file specified by \b _file_name.}
@ent{_EFCLOSE, Could not close file pointer created with \b _file_name.}
@end
@note Inherits errors form file_exists()
@see file_exists()
**/
int file_touch (char* _file_name);
/**
@brief Determine if a file exists.
@param _file_name The path to the file.
@return Upon success, returns 0 if the file does exist and 1 if it does.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@details
@beg{Errors}
@ent{_EPTRNULL, \b _file_name was NULL.}
@ent{_ESTREMPTY, \b _file_name begins with a '\0' character.}
@ent{_EACCESS,
Could not determine existence of file pointer created with \b _file_name.}
@end
@see file_touch
**/
int file_exists (char* _file_name);
