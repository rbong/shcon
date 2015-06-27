int _shcon_create_sem (shcon_t* _shcon);
int _shcon_init_sem (shcon_t* _shcon);
int _shcon_attach_shm (shcon_t* _shcon);
int _shcon_check_shm_ver (shcon_t* _shcon);
int _shcon_wait (int _ms);
int _shcon_timed_out_msg (msg_t* _msg);
int _shcon_has_unread_sem (shcon_t* _shcon);
int _shcon_check_prev_time (shcon_t* _shcon, msg_t* _msg);
int _shcon_set_prev_time (shcon_t* _shcon, msg_t* _msg);
