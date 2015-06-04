/** @file msg.h
@author Roger Bongers
@date June 2 2015
@brief Message processing.
**/

#include <stdlib.h>
#include <sys/msg.h>

#include <err.h>
#include <util.h>

/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MSG_MM
#define MSG_MM
enum _MSG_CONST
{
    //! Maximum size of message. System limit.
    MSG_MAX_SIZE = 4056,
};

enum _MSG_TYPE
{
    //! An init message, where only \b type and \b version are guaranteed.
    MSG_INIT = 1L,
    //! A regular message.
    MSG_NORM = 2L,
    //! A message to disconnect, where only \b type is guaranteed.
    MSG_KILL = 3L,
};

/**
@brief Message metadata.
**/
typedef struct
{
    //! Iteration of the program that created the message.
    long ver;
    //! Date of message creation.
    long date;
    //! Length of the data in bytes.
    int len;
} msg_hdr_t;

/**
@brief A message.
**/
typedef struct
{
    //! Type of message.
    long type;
    //! Message metadata.
    msg_hdr_t hdr;
    //! Message information.
    char* data;
} msg_t;
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

/**
@brief Create a new msg_t.
@return Upon success, returns the address of a msg_t with empty members.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EALLOC, could not allocate space for the msg_t.}
@end
**/
msg_t* msg_t_new (void);
/**
@brief Populates a msg_t.
@param _msg The message to populate.
@param _type If \b _type is 0, populates \b _msg with #msg_type.
If \b _type is 1 or more, populates \b _msg with \b _type.
If \b _type is less than 0, does not populate \b _msg msg_t#type.
@param _hdr If \b _hdr.ver is 0, generates a header for \b _msg msg_t#hdr.
If \b _hdr is 1 or more, populates \b _msg with \b _hdr.
If \b _hdr is less than 0, does not populate \b _msg msg_t#hdr.
@param _data If \b _data is non-NULL,
copies \b _data into \b _msg msg_t#data.
If \b _data is NULL, does not populate \b _msg msg_t#data.
@return Upon success, returns 0 and populates \b _msg.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _msg is NULL.}
@end
@note Inherits errors from msg_gen_hdr().
**/
int msg_t_set (msg_t** _msg, long _type, msg_hdr_t _hdr, char* _data);
/**
@brief Deletes a msg_t.
@details Assumes that \b _msg has been properly created by msg_t_new().
Does nothing if \b _msg or \b *_msg is NULL.
@param _msg The struct to free.
@return Sets \b *_msg to NULL after freeing data.
**/
void msg_t_del (msg_t** _msg);
/**
@brief Generates a msg_hdr_t.
@return Upon success, returns a message header with relevant data.
<br>Upon failure, returns a msg_hdr_t with msg_hdr_t#ver set to 0.
@note Inherets errors from get_sec().
**/
msg_hdr_t msg_gen_hdr (void);
/**
@brief Sets message data.
@param _msg The message to populate with data.
@param _data The data to populate the message with.
@return Upon success, allocates space for \b _msg msg_t#data,
populates it with \b _data, and sets \b _msg msg_t#hdr msg_hdr_t#len.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _msg and/or \b _data are NULL.}
@ent{_EALLOC, could not allocate space for the data.}
@end
**/
int msg_set_data (msg_t* _msg, char* _data);
/**
@brief Converts a msg_t into one chunk of data.
@param _msg The message to convert.
@return Upon success, returns the newly allocated memory with the raw message.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _msg is NULL.}
@ent{_EALLOC, could not allocate space for the raw message.}
@end
@note Inherits errors from msg_to_bin_len().
**/
void* msg_to_bin (msg_t* _msg);
/**
@brief Converts a chunk of data into a msg_t.
@param _bmsg The message to convert.
@return Upon success, returns the newly allocated memory with the message.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _bmsg is NULL.}
@ent{_EALLOC, could not allocate space for msg_t msg_t#data.}
@end
@note Inherits errors from msg_t_new().
**/
msg_t* msg_from_bin (void* _bmsg);
/**
@brief Calculates the total size of a message including data.
@param _msg The message.
@return Upon success, returns the raw message size.
<br>Upon failure, returns NULL, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _msg is NULL.}
@end
**/
int msg_to_bin_len (msg_t* _msg);
