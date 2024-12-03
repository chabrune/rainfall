# level8 - Heap Memory Manipulation

## Analyse du Programme

**Structure**
- Boucle principale avec `fgets(buf, 128, stdin)`
- Commandes disponibles: `auth`, `reset`, `service`, `login`
- Vérification critique: `auth != 0` pour `login`

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

**Obtention du Flag**
```bash
cat /home/user/level9/.pass
```
