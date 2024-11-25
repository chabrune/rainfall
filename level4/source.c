int32_t p(char* arg1)
{
    return printf(format: arg1)
}


uint32_t n()
{
    void buf
    fgets(&buf, n: 0x200, fp: __bss_start)
    p(&buf)
    uint32_t m_1 = m0804848d      
    if (m_1 != 0x1025544)
    return m_1080484a6      
    return system(line: "/bin/cat /home/user/level5/.pass")
}


int32_t main(int32_t argc, char** argv, char** envp)
{
    return n()
}
