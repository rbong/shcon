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
}
enum _MSG_TYPE
{
    //! An init message, where only \b type and \b version are guaranteed.
    MSG_INIT = 1L,
    //! A regular message.
    MSG_NORM = 2L,
    //! A message to disconnect, where only \b type is guaranteed.
    MSG_KILL = 3L,
}
/**
@brief Message metadata.
**/
typedef struct
{
    //! Iteration of the program that created the message.
    long version;
    //! Date of message creation.
    long date;
    //! Length of the data in bytes.
    int data_len;
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
