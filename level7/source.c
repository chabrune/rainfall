int32_t m()
{
    return printf(format: "%s - %d\n", 0x8049960, time(nullptr))
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    int32_t* eax = malloc(bytes: 8)
    eax = 1
    eax[1] = malloc(bytes: 8)
    int32_t* eax_4 = malloc(bytes: 8)
    eax_4 = 2
    eax_4[1] = malloc(bytes: 8)
    strcpy(eax[1], argv[1])
    strcpy(eax_4[1], argv[2])
    fgets(buf: &c, n: 0x44, fp: fopen(filename: "/home/user/level8/.pass", mode: &data_80486e9))
    puts(str: &data_8048703)
    return 0
}