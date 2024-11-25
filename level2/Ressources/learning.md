```
set $ebp+4 = 0xb7e6b060        # Essaie de modifier l'expression elle-même
set *($ebp+4) = 0xb7e6b060     # Écrit à l'adresse, mais sans spécifier la taille
set {int}($ebp+4) = 0xb7e6b060 # Écrit 4 bytes à l'adresse ebp+4
```

Configuration du stack pour l'appel à system
```
(gdb) set {int}($ebp+4)=0xb7e6b060    # Adresse de system
(gdb) set {int}($ebp+8)=0xdeadbeef    # Adresse de retour après system
(gdb) set {int}($ebp+12)=0x0804a008    # Argument pour system (/bin/sh)
```

Pourquoi set `ebp+8` avec une adresse fictive ?

C'est la convention d'appel standard : l'appelant (caller) prépare le stack avec l'adresse de retour et les arguments.

Quand vous voyez call `0x8048xxx`, c'est un appel à la `PLT (Procedure Linkage Table)`. La PLT contient des instructions de saut qui vont chercher l'adresse réelle de la fonction dans la `GOT (Global Offset Table).`

`call 0x80483c0` (adresse PLT de gets)
Cette instruction va chercher l'adresse réelle de gets dans la GOT
L'adresse réelle de gets est bien `0xb7e91e40`

Les adresses dans le binaire commencent par `0x8`
Les adresses réelles des fonctions libc commencent par `0xb`

C'est un mécanisme de résolution dynamique des liens qui permet :
De charger les bibliothèques à des adresses variables
De supporter le `ASLR (Address Space Layout Randomization)`

la vérification de l'adresse commençant par 0xb est en fait une protection similaire à ASLR.

Cette protection empêche l'exploitation directe de la stack en vérifiant si l'adresse de retour pointe vers la stack.

Il est possible d’afficher la mémoire avec la commande x. Cette commande se construit ainsi :
```
x/[nb-bloc][format][bloc-size] [address]

    [nb-bloc] : nombre de blocs à afficher
    [format] : format d’affichage, s (string)/i (instruction machine)/x (hexadecimal)
    [bloc-size] : taille des blocs b(1byte)/h(2bytes)/w(4bytes)/g(8bytes)
    [address] : à partir d’où afficher la mémoire.
```

La commande reverse-continue permet un retour au breakpoint précédent ou en abrege : `r c`