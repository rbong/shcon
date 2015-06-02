/** @file msg.h
@author Roger Bongers
@date June 2 2015
@brief Message processing.
**/

#include <sys/msg.h>

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
If \b _type is less than 0, does not populate \b _msg member \b type.
@param _hdr If \b _hdr is 0, generates a header for \b _msg member \b hdr.
If \b _hdr is 1 or more, populates \b _msg with \b _hdr.
If \b _hdr is less than 0, does not populate \b _msg member \b hdr.
@param _data If \b _data is non-NULL,
copies \b _data into \b _msg member \b data.
If \b _data is NULL, does not populate \b _msg member \b data.
@return Upon success, returns 0 and populates \b _msg.
<br>Upon failure, returns -1, prints errors if necessary, and sets #err_num.
@beg{Errors}
@ent{_EPTRNULL, \b _msg is NULL.}
@end
@note Inherits errors from msg_gen_hdr().
**/
int msg_t_set (msg_t** _msg, long _type, msg_hdr_t _hdr, char* _data);
msg_hdr_t msg_gen_hdr (void);
