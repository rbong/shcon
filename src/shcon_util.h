/** @file shcon_util.h
@author Roger Bongers
@date June 2 2015
@brief Shared connection protocol-based utilities.
**/

#include <shcon.h>

int shcon_create_sem_id (shcon_t* _shcon);

int shcon_attach_sem_id (shcon_t* _shcon);

int shcon_init_sem (shcon_t* _shcon);

int shcon_create_shm_id (shcon_t* _shcon);

int shcon_attach_shm_id (shcon_t* _shcon);

int shcon_init_shm (shcon_t* _shcon);

int shcon_send_shm_msg (shcon_t* _shcon, msg_t* _msg);

msg_t* shcon_recv_shm_msg (shcon_t* _shcon, int _init);

int shcon_sem_mark (shcon_t* _shcon);

int shcon_sem_lock (shcon_t* _shcon);

int shcon_sem_unlock (shcon_t* _shcon);

int shcon_sem_add_con (shcon_t* _shcon);
