# level3 - Format String Exploitation

## Analyse du Programme

**Structure**
- Le `main()` appelle une fonction `v()`
- Utilisation de `gets()` suivi d'un `printf()`
- Variable globale `m` à l'adresse `0x804988c` dans la section .bss
- Comparaison de cette globale avec la valeur 0x40 - si false -> return

**Points Clés**
- Adresse de la variable globale `m`: `0x804988c`
- Buffer à l'adresse: `0xbffff520`
- Format string vulnerability dans l'appel à `printf()`

## Exploitation Format String

**Mécanisme**
(voir Ressources/learning.md)

- `printf()` sans arguments traite les valeurs de la pile comme arguments
- Le formateur `%n` écrit le nombre de caractères affichés à l'adresse spécifiée
- `%4$n` accède directement au 4ème argument sur la pile

**Construction de l'Exploit**
1. Écriture de l'adresse de `m` au début du payload
2. Génération de 60 caractères ecrire la valeur souhaitée
3. Utilisation de `%4$n` pour écrire à l'adresse de `m`

## Exploitation

**Commande d'Exploitation**
```bash
(python -c 'print "\x8c\x98\x04\x08" + "%60c" + "%4$n"'; cat) | ./level3
```

**Obtention du Flag**
```bash
cat /home/user/level4/.pass
```

Cette exploitation fonctionne en modifiant la valeur de la variable globale `m` via une vulnérabilité format string, permettant l'exécution du shell.
