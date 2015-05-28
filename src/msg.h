/** @file msg.h
@author Roger Bongers
@date May 28 2015
@brief Message processing.
@see shcon.h
**/

#include <stdlib.h>
#include <string.h>

#include <err.h>

/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MM_MSG
#define MM_MSG
/**
@brief Contains metadata about msg_t.
@see msg_t
**/
typedef struct
{
    //! Iteration of the program that created the message.
    int version;
    //! Date of message creation.
    int date;
    //! Length of the command string of the messsage.
    int cmd_len;
    //! Length of the data string (command parameters) of the message.
    int data_len;
} msg_hdr_t;
/**
@brief Contains a message.
@see msg_hdr_t
**/
typedef struct
{
    //! The message metadata.
    msg_hdr_t hdr;
    //! The command string.
    char* cmd;
    //! The data string.
    char* data;
} msg_t;
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

/**
@brief Create a new msg_t.
@return Upon success, returns an address of a msg_t with empty members.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EALLOC, Unable to allocate memory for the msg_t.}
@end
**/
msg_t* msg_t_new (void);
/**
@brief Deletes a msg_t.
@details Assumes that \b _msg has been properly created by msg_t_new().
Does nothing if \b _msg or \b *_msg is NULL.
@param _msg The message to free.
@return Sets \b *_msg to NULL after freeing data.
**/
void msg_t_del (msg_t** _msg);
/**
@brief Converts a msg_t into raw data.
@param _msg The message to convert.
@return Upon success, returns an address of a binary message.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _msg or some address within was NULL.}
@ent{_EALLOC, Could not allocate memory for the binary message.}
@ent{_EPTROVERFLOW, Memory could not hold binary message.}
@end
@note Inherits errors from msg_to_bin_len().
**/
void*  msg_to_bin (msg_t* _msg);
/**
@brief Calculates size of raw message data.
@param _msg The message to use for calculation.
@return Upon success, returns the size of the message.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _msg was NULL.}
@end
**/
int msg_to_bin_len (msg_t* _msg);
