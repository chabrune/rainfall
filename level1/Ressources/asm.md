# Main
```
Dump of assembler code for function main:
   0x08048480 <+0>:     push   %ebp
   0x08048481 <+1>:     mov    %esp,%ebp
   0x08048483 <+3>:     and    $0xfffffff0,%esp
   0x08048486 <+6>:     sub    $0x50,%esp
   0x08048489 <+9>:     lea    0x10(%esp),%eax
   0x0804848d <+13>:    mov    %eax,(%esp)
   0x08048490 <+16>:    call   0x8048340 <gets@plt>
   0x08048495 <+21>:    leave  
   0x08048496 <+22>:    ret   
```

# Run
```
Dump of assembler code for function run:
   0x08048444 <+0>:     push   %ebp
   0x08048445 <+1>:     mov    %esp,%ebp
   0x08048447 <+3>:     sub    $0x18,%esp
   0x0804844a <+6>:     mov    0x80497c0,%eax
   0x0804844f <+11>:    mov    %eax,%edx
   0x08048451 <+13>:    mov    $0x8048570,%eax
   0x08048456 <+18>:    mov    %edx,0xc(%esp)
   0x0804845a <+22>:    movl   $0x13,0x8(%esp)
   0x08048462 <+30>:    movl   $0x1,0x4(%esp)
   0x0804846a <+38>:    mov    %eax,(%esp)
   0x0804846d <+41>:    call   0x8048350 <fwrite@plt>
   0x08048472 <+46>:    movl   $0x8048584,(%esp)
   0x08048479 <+53>:    call   0x8048360 <system@plt>
   0x0804847e <+58>:    leave  
   0x0804847f <+59>:    ret
```

# Main function :

`main<+0-+3>:`

preparation des registres et de la stack

`main<+6>:     sub    $0x50,%esp`

Alloue 80 bytes sur la stack pour le buf

`main<+9>:     lea    0x10(%esp),%eax`

lea = load effective adress
Charge l'adresse a esp+10 dans eax qui sera notre pointeur buf

`main<+13>:    mov    %eax,(%esp)`

Charge cette adresse dans esp qui prepare le parametre de la fonction gets

`main<+16>:    call   0x8048340 <gets@plt>`

call gets qui va remplir notre buf avec stdin

# Run function :

`run<+0-+3>:`

preparation des registres et de la stack

`run<+6-+38>:`

prepare les arguments pour fwrite
fwrite(buf: "Good... Wait what?\n", size: 1, count: 0x13, fp: stdout)

```
(gdb) x/s 0x80497c0
0x80497c0 <stdout@@GLIBC_2.0>:   ""
(gdb) x/s 0x8048570
0x8048570:       "Good... Wait what?\n"
0x0804845a <+22>:    movl   $0x13,0x8(%esp)
0x08048462 <+30>:    movl   $0x1,0x4(%esp)
```

Donc on se retrouve avec quelque chose comme :
```
esp+0   = "Good... Wait what?\n"
esp+4   = 1
esp+8   = 0x13
esp+12  = stdout
```

`run<+46>:    movl   $0x8048584,(%esp)`

```
(gdb) x/s 0x8048584
0x8048584:       "/bin/sh"
```
charge le pointeur sur string dans esp pour system

`run<+53>:    call   0x8048360 <system@plt>`

call system avec notre param dans esp
