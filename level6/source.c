int32_t n()
{
    return system(line: "/bin/cat /home/user/level7/.pass")
}

int32_t m()
{
    return puts(str: "Nope")
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    char* eax = malloc(bytes: 0x40)
    int32_t (** eax_1)() = malloc(bytes: 4)
    eax_1 = m
    strcpy(eax, argv[1])
    return (*eax_1)()
}
