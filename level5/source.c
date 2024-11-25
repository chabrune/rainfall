void o() __noreturn
{
    system(line: "/bin/sh")
    _exit(status: 1)
    noreturn
}

void n() __noreturn
{
    void var_20c
    fgets(buf: &var_20c, n: 0x200, fp: __bss_start)
    printf(format: &var_20c)
    exit(status: 1)
    noreturn
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    n()
    noreturn
}
