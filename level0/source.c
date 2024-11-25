int32_t main(void* arg1)
{
    if (atoi(*(arg1 + 4)) != 0x1a7)
        _IO_fwrite("No !\n", 1, 5, _IO_stderr)
    else
    {
        char* var_20 = __strdup("/bin/sh")
        int32_t var_1c_1 = 0
        int32_t eax_5 = __getegid()
        int32_t eax_6 = __geteuid()
        __setresgid(eax_5, eax_5, eax_5)
        __setresuid(eax_6, eax_6, eax_6)
        execv("/bin/sh", &var_20)
    }
    return 0
}