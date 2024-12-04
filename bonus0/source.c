#include <string.h>
#include <stdio.h>
#include <unistd.h>

char* p(char* arg1, char* arg2) {
    char buf[0x1008];  
    
    puts(arg2);
    
    read(0, buf, 0x1000); // 4096
    
    char* newline = strchr(buf, '\n');
    if (newline) *newline = '\0';
    
    strncpy(arg1, buf, 0x14); // copie 20 mais pas de \n
    
    return arg1;
}

char* pp(char* arg1) {
    char buffer1[48];  // [ebp-0x30]
    char buffer2[28];  // [ebp-0x1c]
    
    p(buffer1, data_80486a0);  // Premier input: AAAAA...
    p(buffer2, data_80486a0);  // Deuxième input: BBBBB...
    
    strcpy(arg1, buffer1);     // Copie AAAAA dans arg1
    
    size_t len = strlen(arg1); // Trouve la longueur de arg1
    arg1[len] = ' ';          // Ajoute un espace
    
    strcat(arg1, buffer2);     // Ajoute BBBBB à la suite
}

int main(int argc, char** argv) {
    char str[42];  // [esp+0x16]
    
    pp(str);
    puts(str);
    
    return 0;
}
