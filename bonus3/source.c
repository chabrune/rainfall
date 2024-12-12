#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    FILE *file;
    char buffer[66];
    char content[66];
    
    file = fopen("/home/user/end/.pass", "r");
    
    memset(buffer, 0, 66);
    
    if (!file || argc != 2) {
        return -1;
    }
    
    fread(buffer, 1, 66, file);
    buffer[89] = '\0'; 
    
    buffer[atoi(argv[1])] = '\0';
    
    fread(content, 1, 65, file);
    
    fclose(file);
    
    if (strcmp(buffer, argv[1]) == 0) {
        execl("/bin/sh", "/bin/sh", NULL);
    } else {
        puts(buffer + 66);
    }
    
    return 0;
}
