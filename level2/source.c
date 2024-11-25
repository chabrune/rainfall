char* p()
{
    fflush(fp: stdout)
    void var_50
    gets(buf: &var_50)      
    if ((__return_addr & 0xb0000000) != 0xb0000000)
    {
        puts(str: &var_50)
        return strdup(s: &var_50)
    }
    printf(format: "(%p)\n", __return_addr)
    _exit(status: 1)
    noreturn
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    return p()
}
