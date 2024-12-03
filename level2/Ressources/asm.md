# level2 - Analyse Assembleur Détaillée

## Fonction Main
```nasm
   0x0804853f <+0>:     push   %ebp
   0x08048540 <+1>:     mov    %esp,%ebp
   0x08048542 <+3>:     and    $0xfffffff0,%esp
```
Prologue standard: sauvegarde du frame pointer et alignement

```nasm
   0x08048545 <+6>:     call   0x80484d4 <p>
```
Appel de la fonction p()

```nasm
   0x0804854a <+11>:    leave  
   0x0804854b <+12>:    ret  
```
Épilogue standard

## Fonction P

```nasm
   0x080484d4 <+0>:     push   %ebp
   0x080484d5 <+1>:     mov    %esp,%ebp
   0x080484d7 <+3>:     sub    $0x68,%esp
```
Prologue et allocation de 104 bytes sur la stack

```nasm
   0x080484da <+6>:     mov    0x8049860,%eax
   0x080484df <+11>:    mov    %eax,(%esp)
   0x080484e2 <+14>:    call   0x80483b0 <fflush@plt>
```
Vide le buffer stdout

```nasm
   0x080484e7 <+19>:    lea    -0x4c(%ebp),%eax
   0x080484ea <+22>:    mov    %eax,(%esp)
   0x080484ed <+25>:    call   0x80483c0 <gets@plt>
```
Appel à gets() avec le buffer comme argument

```nasm
   0x080484f2 <+30>:    mov    0x4(%ebp),%eax
   0x080484f5 <+33>:    mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:    mov    -0xc(%ebp),%eax
```
Sauvegarde et récupération de l'adresse de retour

```nasm
   0x080484fb <+39>:    and    $0xb0000000,%eax
   0x08048500 <+44>:    cmp    $0xb0000000,%eax
   0x08048505 <+49>:    jne    0x8048527 <p+83>
```
Vérification si l'adresse de retour pointe vers la stack (commence par 0xb)

```nasm
   0x08048527 <+83>:    lea    -0x4c(%ebp),%eax
   0x0804852a <+86>:    mov    %eax,(%esp)
   0x0804852d <+89>:    call   0x80483f0 <puts@plt>
```
Affichage du buffer avec puts()

```nasm
   0x08048532 <+94>:    lea    -0x4c(%ebp),%eax
   0x08048535 <+97>:    mov    %eax,(%esp)
   0x08048538 <+100>:   call   0x80483e0 <strdup@plt>
```
Duplication du buffer avec strdup()

```nasm
   0x0804853d <+105>:   leave  
   0x0804853e <+106>:   ret  
```
Épilogue standard

