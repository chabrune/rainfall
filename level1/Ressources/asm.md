# level1 - Analyse Assembleur Détaillée

## Fonction Main

```nasm
Dump of assembler code for function main:
   0x08048480 <+0>:     push   %ebp
   0x08048481 <+1>:     mov    %esp,%ebp
   0x08048483 <+3>:     and    $0xfffffff0,%esp
```
Prologue standard: sauvegarde de l'ancien frame pointer et alignement de la stack

```nasm
   0x08048486 <+6>:     sub    $0x50,%esp
```
Allocation de 80 bytes sur la stack pour les variables locales

```nasm
   0x08048489 <+9>:     lea    0x10(%esp),%eax
```
Charge l'adresse effective de esp+16 dans eax (adresse du buffer)

```nasm
   0x0804848d <+13>:    mov    %eax,(%esp)
```
Place l'adresse du buffer comme premier argument de gets()

```nasm
   0x08048490 <+16>:    call   0x8048340 <gets@plt>
```
Appel à gets() qui lit l'entrée standard sans vérification de taille

```nasm
   0x08048495 <+21>:    leave  
   0x08048496 <+22>:    ret   
```
Épilogue standard: restauration du frame pointer et retour

## Fonction Run

```nasm
   0x08048444 <+0>:     push   %ebp
   0x08048445 <+1>:     mov    %esp,%ebp
   0x08048447 <+3>:     sub    $0x18,%esp
```
Prologue standard et allocation de 24 bytes sur la stack

```nasm
   0x0804844a <+6>:     mov    0x80497c0,%eax
   0x0804844f <+11>:    mov    %eax,%edx
```
Charge l'adresse de stdout dans edx

```nasm
   0x08048451 <+13>:    mov    $0x8048570,%eax
```
Charge l'adresse de la chaîne "Good... Wait what?\n" dans eax

```nasm
   0x08048456 <+18>:    mov    %edx,0xc(%esp)
   0x0804845a <+22>:    movl   $0x13,0x8(%esp)
   0x08048462 <+30>:    movl   $0x1,0x4(%esp)
   0x0804846a <+38>:    mov    %eax,(%esp)
```
Prépare les arguments pour fwrite():
- esp+0: pointeur vers la chaîne
- esp+4: taille d'un élément (1)
- esp+8: nombre d'éléments (0x13 = 19)
- esp+12: pointeur vers stdout

```nasm
   0x0804846d <+41>:    call   0x8048350 <fwrite@plt>
```
Appel à fwrite() pour afficher le message

```nasm
   0x08048472 <+46>:    movl   $0x8048584,(%esp)
```
Place l'adresse de la chaîne "/bin/sh" comme argument de system()

```nasm
   0x08048479 <+53>:    call   0x8048360 <system@plt>
```
Appel à system() pour exécuter /bin/sh

```nasm
   0x0804847e <+58>:    leave  
   0x0804847f <+59>:    ret
```
Épilogue standard de la fonction
