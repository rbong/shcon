/** @file shcon_util.h
@author Roger Bongers
@date June 2 2015
@brief Shared connection utilities.
**/

#include <shcon.h>

int shcon_create_sem_id (shcon_t* _shcon);
int shcon_attach_sem_id (shcon_t* _shcon);
int shcon_init_sem (shcon_t* _shcon);
int shcon_create_shm_id (shcon_t* _shcon);
int shcon_attach_shm_id (shcon_t* _shcon);
int shcon_init_shm (shcon_t* _shcon);
int shcon_kill_shm (shcon_t* _shcon);
int shcon_check_shm_init (shcon_t* _shcon);
int shcon_create_sem_shm (shcon_t* _shcon);
int shcon_create_kill_shm (shcon_t* _shcon);
int shcon_attach_sem_shm (shcon_t* _shcon);
