#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int var_3c;
    
    int nb = atoi(argv[1]);
    
    if (nb > 9)
        return 1;
        
    memcpy(&var_3c, argv[2], nb * 4);
    
    if (var_3c == 0x574f4c46)
        execl("/bin/sh", "/bin/sh", NULL);
        
    return 0;
}
