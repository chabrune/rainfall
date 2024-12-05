Il est possible de chercher un octet spécifique avec GDB, mais il faut utiliser le modificateur `/b` pour spécifier que l'on cherche des bytes. 

La commande correcte serait :

```gdb
find /b 0xbffdf000, +2000, 0x6a
```

Sans le modificateur `/b`, GDB interprète la valeur selon le type par défaut du langage courant. Par exemple, en C/C++, une valeur non typée comme 0x6a sera interprétée comme un entier (int), ce qui recherchera généralement 4 bytes.

## Autres options de recherche

Le format général de la commande est :

```gdb
find [/sn] start_addr, +len, val1 [, val2, ...]
```

Où :
- `/b` cherche des bytes (1 octet)
- `/h` cherche des halfwords (2 octets) 
- `/w` cherche des words (4 octets)
- `/g` cherche des giant words (8 octets)

Le nombre de correspondances trouvées est stocké dans la variable `$numfound` et la dernière adresse trouvée dans `$_`.


## Comportement de la Stack Frame

En C, lorsqu'une fonction se termine, ses variables locales ne sont pas réellement "détruites" ou "effacées" de la mémoire. Voici ce qui se passe réellement :

## Mécanisme de la Stack

1. **Allocation de la Stack Frame**
- Quand une fonction est appelée, l'espace est réservé sur la stack
- Les variables locales sont placées dans cet espace
- Le pointeur de pile (ESP) et le base pointer (EBP) sont ajustés

2. **Désallocation**
- Lors du `leave` de la fonction, seuls les pointeurs sont réajustés
- Les données restent physiquement en mémoire
- Aucun "nettoyage" n'est effectué

## Implications pour la Sécurité

Cette caractéristique a plusieurs implications :

1. **Données Résiduelles**
- Les données des fonctions précédentes restent accessibles
- Ces données peuvent être lues ou modifiées si on connaît leur emplacement
- C'est pourquoi on peut trouver des informations sensibles dans la stack

2. **Exploitation**
- Les attaques de type buffer overflow peuvent utiliser ces données résiduelles
- Le shellcode peut être placé dans une variable locale d'une fonction
- Même après le retour de la fonction, le shellcode reste exécutable

## Bonnes Pratiques

Pour se protéger :
- Initialiser les variables locales à zéro
- Utiliser des fonctions qui effacent explicitement la mémoire (comme `memset`)
- Éviter de stocker des données sensibles dans les variables locales

Cette compréhension est cruciale pour la sécurité des applications et l'analyse de vulnérabilités.