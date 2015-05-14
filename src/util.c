#include <util.h>
// add better error checking
int nc_create_file (char* file_name)
{
    FILE* fp;

    fp = fopen (file_name, "a+");
    fclose (fp);

    return 1;
}
