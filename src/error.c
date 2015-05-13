void error_set (int code)
{
    error_number = code;
}

char* error_get_msg (int code)
{
    if (code >= ERROR_MAX || code < 0)
    {
        return error_table [_EUNKNOWN].msg;
    }
    return error_table [code].msg;
}
