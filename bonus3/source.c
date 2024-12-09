#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    FILE *file;
    char buffer[66];  // 0x42 bytes
    char content[66]; // Pour stocker le contenu du fichier
    
    // Ouvre le fichier .pass
    file = fopen("/home/user/end/.pass", "r");
    
    // Initialise le buffer à 0
    memset(buffer, 0, 66);
    
    // Vérifie si le fichier est ouvert et si on a 2 arguments
    if (!file || argc != 2) {
        return -1;
    }
    
    // Lit les 66 premiers bytes du fichier
    fread(buffer, 1, 66, file);
    buffer[89] = '\0';  // Termine la chaîne
    
    // Met un \0 à la position donnée par argv[1]
    buffer[atoi(argv[1])] = '\0';
    
    // Lit 65 bytes supplémentaires dans content
    fread(content, 1, 65, file);
    
    // Ferme le fichier
    fclose(file);
    
    // Compare buffer avec argv[1]
    if (strcmp(buffer, argv[1]) == 0) {
        execl("/bin/sh", "/bin/sh", NULL);
    } else {
        puts(buffer + 66);
    }
    
    return 0;
}
