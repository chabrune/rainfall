# level0 - Analyse et Exploitation

Le challenge nécessite une analyse du binaire avec GDB pour comprendre le mécanisme de vérification et obtenir les privilèges du level1.

## Analyse du Programme

**Observation des Instructions Clés**
```nasm
0x08048ed4 <+20>:    call   0x8049710 <atoi>
0x08048ed9 <+25>:    cmp    $0x1a7,%eax
0x08048ede <+30>:    jne    0x8048f58 <main+152>
```

Le programme effectue les opérations suivantes :
- Conversion de l'argument en entier avec `atoi`
- Comparaison avec la valeur `0x1a7` (423 en décimal)
- Saut vers un write("No !\n") puis return si comparaison fausse

## Exploitation

**Méthode**
1. Le programme attend la valeur 423 comme argument
2. Si la comparaison est égale, le programme exécute `/bin/sh` avec les privilèges de level1
3. Les droits SUID sont confirmés par les permissions du binaire : `-rwsr-x---+ 1 level1 users`

**Exécution**
```bash
./level0 423
```

**Vérification**
```bash
whoami
```
La commande confirme l'obtention des privilèges level1.

## Obtention du Flag
```bash
cat /home/user/level1/.pass
```

Cette commande permet de récupérer le mot de passe pour le niveau suivant.
