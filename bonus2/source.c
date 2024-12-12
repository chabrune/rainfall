#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int language = 0;

void greetuser(void) {
    char str[64];
    
    if (language == 1) {
        memcpy(str, "\x48\x79\x76\xc3\xa4\xc3\xa4\x20\x70\xc3\xa4\x69\x76\xc3\xa4\xc3\xa4\x20", 0x12);
        str[0x12] = '\0';
    }
    else if (language == 2) {
        strcpy(str, "Goedemiddag! ");
    }
    else if (language == 0) {
        strncpy(str, "Hello ", 7);
    }
    
    strcat(str, &str[76]); 
    puts(str);
}

int main(int argc, char **argv) {
    char s[76];        // 0x4c bytes
    char var_38[40];   // Variable locale pour stocker argv[2]
    char *env;
    
    if (argc != 3)
        return 1;
        
    memset(s, 0, 76);
    
    strncpy(s, argv[1], 40);        // 0x28 bytes
    strncpy(var_38, argv[2], 32);   // 0x20 bytes
    
    env = getenv("LANG");
    if (env != NULL) {
        if (memcmp(env, "fi", 2) == 0)
            language = 1;
        else if (memcmp(env, "nl", 2) == 0)
            language = 2;
    }
    
    memcpy(&s[76], var_38, 76);
    greetuser();
    
    return 0;
}
