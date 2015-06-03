/** @file shcon_protocol.h
@author Roger Bongers
@date June 2 2015
@brief Shared connection protocol functions.
**/

#include <shcon.h>

int shcon_create_sem_id (shcon_t* _shcon);

int shcon_attach_sem_id (shcon_t* _shcon);

int shcon_init_sem (shcon_t* _shcon);

int shcon_create_shm_id (shcon_t* _shcon);

int shcon_attach_shm_id (shcon_t* _shcon);

int shcon_init_shm (shcon_t* _shcon);

int shcon_kill_shm (shcon_t* _shcon);

int shcon_send_shm_msg (shcon_t* _shcon, msg_t* _msg);

int shcon_check_shm_init (shcon_t* _shcon);

msg_t* shcon_recv_shm_msg (shcon_t* _shcon, int _init);

int shcon_mark_sem (shcon_t* _shcon);

int shcon_lock_sem (shcon_t* _shcon);

int shcon_unlock_sem (shcon_t* _shcon);

int shcon_add_sem_con (shcon_t* _shcon);
