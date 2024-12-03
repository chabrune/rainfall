# level5 - Analyse Assembleur Détaillée

## Fonction Main
```nasm
   0x08048504 <+0>:     push   %ebp
   0x08048505 <+1>:     mov    %esp,%ebp
   0x08048507 <+3>:     and    $0xfffffff0,%esp
```
Prologue standard: sauvegarde du frame pointer et alignement

```nasm
   0x0804850a <+6>:     call   0x80484c2 <n>
```
Appel de la fonction n()

## Fonction N
```nasm
   0x080484c2 <+0>:     push   %ebp
   0x080484c3 <+1>:     mov    %esp,%ebp
   0x080484c5 <+3>:     sub    $0x218,%esp
```
Prologue et allocation de 536 bytes sur la stack

```nasm
   0x080484cb <+9>:     mov    0x8049848,%eax
   0x080484d0 <+14>:    mov    %eax,0x8(%esp)
   0x080484d4 <+18>:    movl   $0x200,0x4(%esp)
   0x080484dc <+26>:    lea    -0x208(%ebp),%eax
   0x080484e2 <+32>:    mov    %eax,(%esp)
```
Préparation des arguments pour fgets:
- esp+8: stdin
- esp+4: taille (512 bytes)
- esp: buffer

```nasm
   0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>
```
Lecture de l'entrée avec fgets

```nasm
   0x080484ea <+40>:    lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:    mov    %eax,(%esp)
   0x080484f3 <+49>:    call   0x8048380 <printf@plt>
```
Appel à printf avec le buffer comme format string

```nasm
   0x080484f8 <+54>:    movl   $0x1,(%esp)
   0x080484ff <+61>:    call   0x80483d0 <exit@plt>
```
Appel à exit(1)

## Fonction O
```nasm
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x18,%esp
```
Prologue et allocation de 24 bytes sur la stack

```nasm
   0x080484aa <+6>:     movl   $0x80485f0,(%esp)
   0x080484b1 <+13>:    call   0x80483b0 <system@plt>
```
Appel à system() avec une commande

```nasm
   0x080484b6 <+18>:    movl   $0x1,(%esp)
   0x080484bd <+25>:    call   0x8048390 <_exit@plt>
```
Appel à _exit(1)

