
/* ------------------------- START OF GUARD BLOCK ------------------------- */
#ifndef MSG_MM
#define MSG_MM
#include <stdlib.h>
#include <sys/msg.h>

#include <err.h>
#include <util.h>

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
    //! A message to quit the thread.
    MSG_QUIT = 4L,
    //! An empty message, not to be parsed.
    MSG_EMPT = 5L,
};

struct msg_hdr_t
{
    //! Iteration of the program that created the message.
    long ver;
    //! Date of message creation.
    long date;
    //! Number of seconds to keep message in the outgoing queue.
    long timeout;
    //! Length of the data in bytes.
    int len;
};

struct msg_t
{
    //! Type of message.
    long type;
    //! Message metadata.
    msg_hdr_t hdr;
    //! Message information.
    char* data;
};
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

typedef struct msg_hdr_t msg_hdr_t;
typedef struct msg_t msg_t;

msg_t* msg_t_new (void);
int msg_t_set (msg_t** _msg, long _type, msg_hdr_t _hdr, char* _data);
void msg_t_del (msg_t** _msg);
msg_hdr_t msg_gen_hdr (void);
int msg_set_data (msg_t* _msg, char* _data);
void* msg_to_raw (msg_t* _msg);
msg_t* msg_from_raw (void* _bmsg);
int msg_to_raw_len (msg_t* _msg);
