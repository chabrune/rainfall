char* p(char* arg1, char* arg2) {
    char buf[0x1008];  // Taille du buffer: 4104 bytes
    
    puts(arg2);
    
    // Lecture de l'entrée standard (fd = 0)
    read(0, buf, 0x1000);  // Lit jusqu'à 4096 bytes
    
    // Trouve et remplace le caractère newline par null
    char* newline = strchr(buf, '\n');
    if (newline) *newline = '\0';
    
    // Copie au maximum 20 (0x14) caractères dans arg1
    strncpy(arg1, buf, 0x14);
    
    return arg1;
}

char* pp(char* arg1) {
    char buffer1[48];  // [ebp-0x30]
    char buffer2[28];  // [ebp-0x1c]
    
    p(buffer1, data_80486a0);
    p(buffer2, data_80486a0);
    
    // Copie buffer1 dans arg1
    strcpy(arg1, buffer1);
    
    // Ajoute un espace à la fin
    size_t len = strlen(arg1);
    arg1[len] = ' ';
    
    // Concatène buffer2 à la fin
    strcat(arg1, buffer2);
    
    return arg1;
}

int main(int argc, char** argv) {
    char str[42];  // [esp+0x16]
    
    pp(str);
    puts(str);
    
    return 0;
}
