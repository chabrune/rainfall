#include <string.h>
#include <stdio.h>
#include <unistd.h>

char* p(char* arg1, char* arg2) {
    char buf[0x1008];  // 4104 bytes sur la stack

    puts(arg2);  // Affiche " - "
    read(0, buf, 0x1000);

    char* newline = strchr(buf, '\n');
    if (newline) *newline = '\0';

    strncpy(arg1, buf, 0x14);
    return arg1;
}

char* pp(char* arg1) {
    char buffer1[48];  // [ebp-0x30]
    char buffer2[28];  // [ebp-0x1c]
    
    p(buffer1, " - ");
    p(buffer2, " - ");

    strcpy(arg1, buffer1);

    size_t len = strlen(arg1);
    arg1[len] = ' ';

    strcat(arg1, buffer2);
    
    return arg1;
}

int main(int argc, char** argv) {
    char str[42];  // [esp+0x16] - Trop petit pour contenir les deux buffers
    
    pp(str);
    puts(str);
    
    return 0;
}
