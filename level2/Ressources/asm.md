# Main
```
(gdb) disas main
Dump of assembler code for function main:
   0x0804853f <+0>:     push   %ebp
   0x08048540 <+1>:     mov    %esp,%ebp
   0x08048542 <+3>:     and    $0xfffffff0,%esp
   0x08048545 <+6>:     call   0x80484d4 <p>
   0x0804854a <+11>:    leave  
   0x0804854b <+12>:    ret  
```

# P
```
Dump of assembler code for function p:
   0x080484d4 <+0>:     push   %ebp
   0x080484d5 <+1>:     mov    %esp,%ebp
   0x080484d7 <+3>:     sub    $0x68,%esp
   0x080484da <+6>:     mov    0x8049860,%eax
   0x080484df <+11>:    mov    %eax,(%esp)
   0x080484e2 <+14>:    call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:    lea    -0x4c(%ebp),%eax
   0x080484ea <+22>:    mov    %eax,(%esp)
   0x080484ed <+25>:    call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:    mov    0x4(%ebp),%eax
   0x080484f5 <+33>:    mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:    mov    -0xc(%ebp),%eax
   0x080484fb <+39>:    and    $0xb0000000,%eax
   0x08048500 <+44>:    cmp    $0xb0000000,%eax
   0x08048505 <+49>:    jne    0x8048527 <p+83>
   0x08048507 <+51>:    mov    $0x8048620,%eax
   0x0804850c <+56>:    mov    -0xc(%ebp),%edx
   0x0804850f <+59>:    mov    %edx,0x4(%esp)
   0x08048513 <+63>:    mov    %eax,(%esp)
   0x08048516 <+66>:    call   0x80483a0 <printf@plt>
   0x0804851b <+71>:    movl   $0x1,(%esp)
   0x08048522 <+78>:    call   0x80483d0 <_exit@plt>
   0x08048527 <+83>:    lea    -0x4c(%ebp),%eax
   0x0804852a <+86>:    mov    %eax,(%esp)
   0x0804852d <+89>:    call   0x80483f0 <puts@plt>
   0x08048532 <+94>:    lea    -0x4c(%ebp),%eax
   0x08048535 <+97>:    mov    %eax,(%esp)
   0x08048538 <+100>:   call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:   leave  
   0x0804853e <+106>:   ret  
```

# Main function

`main<+0-+3>:`

preparation des registres et de la stack

`main<+6>:     call   0x80484d4 <p>`

call fonction p()

# P function

`p<+0-+1>:`

preparation des registres et de la stack

`p<+3>:     sub    $0x68,%esp`

alloue d104 d'espace sur la stack pour buf

`p<+6>:     mov    0x8049860,%eax`
`p<+11>:    mov    %eax,(%esp)`

charge stdout dans eax puis dans esp pour preparer l'arg pour le call a fflush

`p<+14>:    call   0x80483b0 <fflush@plt>`

```
La fonction fflush sert à vider un buffer (tampon) en mémoire en forçant l'écriture immédiate des données qu'il contient vers leur destination finale
```


`p<+19>:    lea    -0x4c(%ebp),%eax`

`p<+22>:    mov    %eax,(%esp)`

prepare le pointeur et le place dans esp pour l'arg de gets

`p<+25>:    call   0x80483c0 <gets@plt>`

call gets avec buf

`p<+30>:    mov    0x4(%ebp),%eax`

l'adresse de retour est charge dans eax

`p<+33>:    mov    %eax,-0xc(%ebp)`

save sur la stack dans $ebp-0xc (les adresses negatives de EBP sont sur la stack)

`p<+36>:    mov    -0xc(%ebp),%eax`

remis dans eax pour la comparaison

`p<+39>:    and    $0xb0000000,%eax`

operation AND logique sur les bits, si l'adresse pointee par eax commence par 0xb, cela signifie que l'adresse est dans la plage 0xb0000000-0xbfffffff

`p<+44>:    cmp    $0xb0000000,%eax`

effectue la comparaison - protection qui verifie si l'adresse de retour pointe vers la stack (qui commence généralement par 0xb en 32-bits)

`p<+49>:    jne    0x8048527 <p+83>`

si le resultat de la comparaison n'est pas egal alors jump a main+83

`p<+51>:    mov    $0x8048620,%eax`

`p<+56>:    mov    -0xc(%ebp),%edx`

`p<+59>:    mov    %edx,0x4(%esp)`

`p<+63>:    mov    %eax,(%esp)`

`p<+66>:    call   0x80483a0 <printf@plt>`

prepare les arguments pour printf puis call printf

`p<+71>:    movl   $0x1,(%esp)`

charge 1 dans esp pour le param de exit


`p<+78>:    call   0x80483d0 <_exit@plt>`

exit 1

`p<+83>:    lea    -0x4c(%ebp),%eax`

charge dans eax l'adresse pointee par ebp-4c qui a ete save avant la comparaison avec 0xb

`p<+86>:    mov    %eax,(%esp)`

`p<+89>:    call   0x80483f0 <puts@plt>`

prepare l'arg pour puts


`p<+94>:    lea    -0x4c(%ebp),%eax`

`p<+97>:    mov    %eax,(%esp)`

`p<+100>:   call   0x80483e0 <strdup@plt>`

effectue les memes operations pour strdup