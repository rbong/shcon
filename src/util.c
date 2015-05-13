int cat_strings (char* dest, char* pre, char* post)
{
    if (size (dest) < cat_length (pre, post))
    {
        ext_errno = STR_SMALL;
        return -1;
    }

    // add better error checking
    strcpy (dest, pre);
    strcpy (dest + strlen (pre), post);

    return 1;
}

int cat_length (char* pre, char* post)
{
    return strlen (pre) + strlen (post) + 1;
}

// add better error checking
int nc_create_file (char* file_name)
{
    FILE* fp;

    fp = fopen (file_name, "a+");
    fclose (fp);

    return 1;
}
