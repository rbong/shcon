
#define ERR_PRINT(n) (_err_pr (stderr, _ERR_LOC, _err_get_msg (err_num = n)))
typedef struct
{
    //! The file where the error occured.
    char* file;
    //! The function where the error occured.
    const char* func;
    //! The line where the error occured.
    int line;
} err_loc_t;



extern const int ERR_MAX;
extern int err_verbose;
#endif
/* -------------------------- END OF GUARD BLOCK -------------------------- */

typedef struct err_info_t err_info_t;

char* _err_get_msg (int _code);
void _err_from (FILE* _fp, err_loc_t _loc);
