uint32_t v()
{
    void var_20c
    fgets(buf: &var_20c, n: 0x200, fp: stdin)
    printf(format: &var_20c)
    uint32_t m_1 = m
    
    if (m_1 != 0x40)
        return m_1
    
    fwrite(buf: "Wait what?!\n", size: 1, count: 0xc, fp: stdout)
    return system(line: "/bin/sh")
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    return v()
}
