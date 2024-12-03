# Main
```nasm
Dump of assembler code for function main:
   0x08048521 <+0>:     push   %ebp
   0x08048522 <+1>:     mov    %esp,%ebp
   0x08048524 <+3>:     and    $0xfffffff0,%esp
   0x08048527 <+6>:     sub    $0x20,%esp
   0x0804852a <+9>:     movl   $0x8,(%esp)
   0x08048531 <+16>:    call   0x80483f0 <malloc@plt>
   0x08048536 <+21>:    mov    %eax,0x1c(%esp)
   0x0804853a <+25>:    mov    0x1c(%esp),%eax
   0x0804853e <+29>:    movl   $0x1,(%eax)
   0x08048544 <+35>:    movl   $0x8,(%esp)
   0x0804854b <+42>:    call   0x80483f0 <malloc@plt>
   0x08048550 <+47>:    mov    %eax,%edx
   0x08048552 <+49>:    mov    0x1c(%esp),%eax
   0x08048556 <+53>:    mov    %edx,0x4(%eax)
   0x08048559 <+56>:    movl   $0x8,(%esp)
   0x08048560 <+63>:    call   0x80483f0 <malloc@plt>
   0x08048565 <+68>:    mov    %eax,0x18(%esp)
   0x08048569 <+72>:    mov    0x18(%esp),%eax
   0x0804856d <+76>:    movl   $0x2,(%eax)
   0x08048573 <+82>:    movl   $0x8,(%esp)
   0x0804857a <+89>:    call   0x80483f0 <malloc@plt>
   0x0804857f <+94>:    mov    %eax,%edx
   0x08048581 <+96>:    mov    0x18(%esp),%eax
   0x08048585 <+100>:   mov    %edx,0x4(%eax)
   0x08048588 <+103>:   mov    0xc(%ebp),%eax
   0x0804858b <+106>:   add    $0x4,%eax
   0x0804858e <+109>:   mov    (%eax),%eax
   0x08048590 <+111>:   mov    %eax,%edx
   0x08048592 <+113>:   mov    0x1c(%esp),%eax
   0x08048596 <+117>:   mov    0x4(%eax),%eax
   0x08048599 <+120>:   mov    %edx,0x4(%esp)
   0x0804859d <+124>:   mov    %eax,(%esp)
   0x080485a0 <+127>:   call   0x80483e0 <strcpy@plt>
   0x080485a5 <+132>:   mov    0xc(%ebp),%eax
   0x080485a8 <+135>:   add    $0x8,%eax
   0x080485ab <+138>:   mov    (%eax),%eax
   0x080485ad <+140>:   mov    %eax,%edx
   0x080485af <+142>:   mov    0x18(%esp),%eax
   0x080485b3 <+146>:   mov    0x4(%eax),%eax
   0x080485b6 <+149>:   mov    %edx,0x4(%esp)
   0x080485ba <+153>:   mov    %eax,(%esp)
   0x080485bd <+156>:   call   0x80483e0 <strcpy@plt>
   0x080485c2 <+161>:   mov    $0x80486e9,%edx
   0x080485c7 <+166>:   mov    $0x80486eb,%eax
   0x080485cc <+171>:   mov    %edx,0x4(%esp)
   0x080485d0 <+175>:   mov    %eax,(%esp)
   0x080485d3 <+178>:   call   0x8048430 <fopen@plt>
   0x080485d8 <+183>:   mov    %eax,0x8(%esp)
   0x080485dc <+187>:   movl   $0x44,0x4(%esp)
   0x080485e4 <+195>:   movl   $0x8049960,(%esp)
   0x080485eb <+202>:   call   0x80483c0 <fgets@plt>
   0x080485f0 <+207>:   movl   $0x8048703,(%esp)
   0x080485f7 <+214>:   call   0x8048400 <puts@plt>
   0x080485fc <+219>:   mov    $0x0,%eax
   0x08048601 <+224>:   leave  
   0x08048602 <+225>:   ret   
```

# M
```nasm
Dump of assembler code for function m:
   0x080484f4 <+0>:     push   %ebp
   0x080484f5 <+1>:     mov    %esp,%ebp
   0x080484f7 <+3>:     sub    $0x18,%esp
   0x080484fa <+6>:     movl   $0x0,(%esp)
   0x08048501 <+13>:    call   0x80483d0 <time@plt>
   0x08048506 <+18>:    mov    $0x80486e0,%edx
   0x0804850b <+23>:    mov    %eax,0x8(%esp)
   0x0804850f <+27>:    movl   $0x8049960,0x4(%esp)
   0x08048517 <+35>:    mov    %edx,(%esp)
   0x0804851a <+38>:    call   0x80483b0 <printf@plt>
   0x0804851f <+43>:    leave  
   0x08048520 <+44>:    ret    
```

# Main function

```nasm
<main+0>:     push   %ebp
```

```nasm
<main+1>:     mov    %esp,%ebp
```

```nasm
<main+3>:     and    $0xfffffff0,%esp
```

Comme d'hab, preparation de la stack frame pour la fonction  

```nasm
<main+6>:     sub    $0x20,%esp
```

32 bytes pour variables locales

```nasm
<main+9>:  movl   $0x8,(%esp)
```

charge l'adresse pointe par esp a 8 (argument pour malloc)
esp            0xbffff710 
```nasm
(gdb) x/x 0xbffff710
0xbffff710:     0x00000008
```

```nasm
<main+16>: call   0x80483f0 <malloc@plt>
```

Alloue 8 bytes sur la heap

```nasm
<main+21>: mov    %eax,0x1c(%esp)
```

On charge l'adresse de retour de malloc(le pointeur sur la heap) dans esp+28
```nasm
eax            0x804a008
(gdb) x/x $esp+28
0xbffff72c:     0x0804a008
```

```nasm
<main+25>: mov    0x1c(%esp),%eax
```

On remet cette adresse dans eax

C'est une pratique courante en assembleur de sauvegarder les valeurs importantes en mémoire plutôt que de compter sur la préservation des registres.

```nasm
<main+29>:    movl   $0x1,(%eax)
```

```nasm
<main+35>:    movl   $0x8,(%esp)
```

Charge la valeur 1 a l'adresse pointe par eax (0x0804a008)
et charge l'adresse pointe par esp a 8 pour l'arg du next malloc()
```nasm
x/x 0x804a008
0x804a008:      0x00000001
x/x $esp
0xbffff710:     0x00000008
```

```nasm
<main+42>: call   0x80483f0 <malloc@plt>
```

Alloue 8 bytes sur la heap

```nasm
<main+47>:    mov    %eax,%edx
```

charge le contenu de eax dans edx (le pointeur sur la heap)
```nasm
eax            0x804a018
edx            0x804a018
```
```nasm
<main+49>:    mov    0x1c(%esp),%eax
```

charge le contenu de esp+28 (l'adresse de retour du premier malloc(0x804a008)) dans eax

```nasm
<main+53>:    mov    %edx,0x4(%eax)
```

charge cette adresse dans eax+4 (0x804a00c)
```nasm
(gdb) x/x 0x804a00c
0x804a00c:      0x0804a018
```

```nasm
<main+56>:    movl   $0x8,(%esp)
```

charge 8 dans l'adresse pointe par esp 
```nasm
esp            0xbffff700 
(gdb) x/x 0xbffff700
0xbffff700:     0x00000008
```

```nasm
<main+63>:    call   0x80483f0 <malloc@plt>
```

alloue 8 bytes sur la heap

```nasm
<main+68>:    mov    %eax,0x18(%esp)
```

charge l'adresse de retour de malloc (0x804a028) a esp+24
```nasm
(gdb) x/x $esp+24
0xbffff718:     0x0804a028
```

```nasm
<main+72>:    mov    0x18(%esp),%eax
```

??? purkwa fer frr

```nasm
<main+76>:    movl   $0x2,(%eax)
```

charge la valeur 2 dans l'adresse pointee par eax (0x0804a028)
```nasm
(gdb) x/x 0x0804a028
0x804a028:      0x00000002
```

```nasm
<main+82>:    movl   $0x8,(%esp)
```

rebelotte charge 8 dans $esp pour next malloc

```nasm
<main+89>:    call   0x80483f0 <malloc@plt>
```

alloue 8 bytes sur la heap

```nasm
<main+94>:    mov    %eax,%edx
```

charge l'adresse de retour de malloc dans edx
```nasm
eax            0x804a038
edx            0x804a038
```

```nasm
<main+96>:    mov    0x18(%esp),%eax
```

charge l'adresse du 2e malloc (0x804a028) dans eax

```nasm
<main+100>:   mov    %edx,0x4(%eax)
```

charge adresse du 3eme malloc (0x804a038) dans $eax+4

```nasm
<main+103>:   mov    0xc(%ebp),%eax
```
```nasm
<main+106>:   add    $0x4,%eax
```

charge argv dans eax puis ajoute 4 pour pointer sur argv[1]
eax            0xbffff7c4
+4
eax            0xbffff7c8

```nasm
ebp+0 old ebp
ebp+4 return
ebp+8 argc
ebp+12 argv
```

```nasm
<main+109>:   mov    (%eax),%eax
```

charge l'adresse pointe par eax dans eax
```nasm
eax            0xbffff7c8
(gdb) x/x 0xbffff7c8
0xbffff7c8:     0xbffff905
-->
eax            0xbffff905
```

```nasm
<main+111>:   mov    %eax,%edx
```

charge cette adresse dans edx
edx            0xbffff905

```nasm
<main+113>:   mov    0x1c(%esp),%eax
```

charge l'adresse contenu a esp+28 dans eax
```nasm
(gdb) x/x $esp+28
0xbffff71c:     0x0804a008
eax            0x804a008
```

```nasm
<main+117>:   mov    0x4(%eax),%eax
```

eax            0x804a018

```nasm
<main+120>:   mov    %edx,0x4(%esp)
```

set esp+4 (2eme arg pour strcpy) avec 0xbffff905

```nasm
<main+124>:   mov    %eax,(%esp)
```

set esp avec (1er arg) avec 0x804a018

```nasm
<main+127>:   call   0x80483e0 <strcpy@plt>
```

strcpy(dest, source)
Donc copie argv[1] dans le buf pointant sur 0x804a018
```nasm
x/2wx $esp
0xbffff700:     0x0804a018      0xbffff905
```

```nasm
<main+132>:   mov    0xc(%ebp),%eax
```

charge eax avec argv

```nasm
<main+135>:   add    $0x8,%eax
```

ajoute 8 bytes pour deplacer le pointeur a argv[2]

```nasm
<main+138>:   mov    (%eax),%eax
```

eax            0xbffff908
0xbffff908 pointeur sur argv[2]

```nasm
<main+140>:   mov    %eax,%edx
```

charge edx avec 0xbffff908


```nasm
<main+142>:   mov    0x18(%esp),%eax
```

eax            0x804a028
adresse 3e malloc

```nasm
<main+146>:   mov    0x4(%eax),%eax
```

(gdb) x/x $eax+4
0x804a02c:      0x0804a038

```nasm
<main+149>:   mov    %edx,0x4(%esp)
```

charge esp+4 avec 0xbffff908 pour arg2 strcpy

```nasm
<main+153>:   mov    %eax,(%esp)
```

charge esp avec 0x0804a038 pour arg1 strcpy

```nasm
<main+156>:   call   0x80483e0 <strcpy@plt>
```

strcyp(d, s) 
copie argv[2] dans le 4e malloc

```nasm
<main+161>:   mov    $0x80486e9,%edx
```

(gdb) x/s 0x80486e9
0x80486e9:       "r"

charge "r" dans edx
pour l'option read

```nasm
<main+166>:   mov    $0x80486eb,%eax
```

charge "/home/user/level8/.pass" dans eax

```nasm
(gdb) x/s 0x80486eb
0x80486eb:       "/home/user/level8/.pass"
```

```nasm
<main+171>:   mov    %edx,0x4(%esp)
```

esp+4 = r (2e arg)

```nasm
<main+175>:   mov    %eax,(%esp)
```

esp = "/home/user/level8/.pass"

```nasm
<main+178>:   call   0x8048430 <fopen@plt>
```

open(file, r)
return un pointeur (*FILE) sur le fichier en cas de succes

```nasm
<main+183>: mov %eax,0x8(%esp)
```

Place le pointeur FILE retourné par fopen comme 3ème argument pour fgets

```nasm
<main+187>: movl $0x44,0x4(%esp)
```

Place 68 (0x44) comme 2ème argument pour fgets (taille à lire) 

```nasm
<main+195>: movl $0x8049960,(%esp)
```

Place l'adresse du buffer comme 1er argument pour fgets 

```nasm
<main+202>: call 0x80483c0 <fgets@plt>
```

fgets(buffer, 68, file) - Lit jusqu'a 68 caracteres du fichier

```nasm
<main+207>: movl $0x8048703,(%esp)
```

Place une chaine de caracteres comme argument pour puts 
```nasm
(gdb) x/s 0x8048703
0x8048703:       "~~"
```

```nasm
<main+214>: call 0x8048400 <puts@plt>
```

Affiche la chaine 

```nasm
<main+219>: mov $0x0,%eax
```

Place 0 comme valeur de retour



# M function

```nasm
<m+3>: sub $0x18,%esp
```

réserve 24 bytes sur la stack

```nasm
<m+6>: movl $0x0,(%esp)
```

Place 0 comme argument pour time() 

```nasm
<m+13>: call 0x80483d0 <time@plt>
```

Appelle time(0) pour obtenir le timestamp actuel

```nasm
<m+18>: mov $0x80486e0,%edx
```

Charge une chaine de format dans edx
(gdb) x/s 0x80486e0
0x80486e0:       "%s - %d\n"

```nasm
<m+23>: mov %eax,0x8(%esp)
```

Place le tim0x0804a018
Place une adresse comme 2ème argument pour printf 
(gdb) x/s 0x8049960
0x8049960 <c>:   ""

```nasm
<m+35>: mov %edx,(%esp)
```

Place la chaîne de format comme 1er argument

```nasm
<m+38>: call 0x80483b0 <printf@plt>
```

Appelle printf avec les arguments préparés 
