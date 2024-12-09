Site incroyable sur PLT/GOT/Linkage/Lib 
`https://www.segmentationfault.fr/linux/role-plt-got-ld-so/`

```nasm
(gdb) info files
0x08048370 - 0x080483f0 is .plt
0x08049814 - 0x08049818 is .got
0x08049818 - 0x08049840 is .got.plt
```

# PLT
```nasm
(gdb) disas 0x08048370, 0x080483f0
Dump of assembler code from 0x8048370 to 0x80483f0:
   0x08048370:  pushl  0x804981c
   0x08048376:  jmp    *0x8049820
   0x0804837c:  add    %al,(%eax)
   0x0804837e:  add    %al,(%eax)
   0x08048380 <printf@plt+0>:   jmp    *0x8049824
   0x08048386 <printf@plt+6>:   push   $0x0
   0x0804838b <printf@plt+11>:  jmp    0x8048370
   0x08048390 <_exit@plt+0>:    jmp    *0x8049828
   0x08048396 <_exit@plt+6>:    push   $0x8
   0x0804839b <_exit@plt+11>:   jmp    0x8048370
   0x080483a0 <fgets@plt+0>:    jmp    *0x804982c
   0x080483a6 <fgets@plt+6>:    push   $0x10
   0x080483ab <fgets@plt+11>:   jmp    0x8048370
   0x080483b0 <system@plt+0>:   jmp    *0x8049830
   0x080483b6 <system@plt+6>:   push   $0x18
   0x080483bb <system@plt+11>:  jmp    0x8048370
   0x080483c0 <__gmon_start__@plt+0>:   jmp    *0x8049834
   0x080483c6 <__gmon_start__@plt+6>:   push   $0x20
   0x080483cb <__gmon_start__@plt+11>:  jmp    0x8048370
   0x080483d0 <exit@plt+0>:     jmp    *0x8049838
   0x080483d6 <exit@plt+6>:     push   $0x28
   0x080483db <exit@plt+11>:    jmp    0x8048370
   0x080483e0 <__libc_start_main@plt+0>:        jmp    *0x804983c
   0x080483e6 <__libc_start_main@plt+6>:        push   $0x30
   0x080483eb <__libc_start_main@plt+11>:       jmp    0x8048370
```

- on constate que la section .plt est composée de plusieurs sous-sections de taille égale (0x10)
- En regardant de plus près chaque pointeur, y compris celui de l’entrée 0, on s’aperçoit qu’ils se trouvent tous les uns à la suite des autres

- Ces adresses se situent donc dans la section nommée .got.plt. Les régions nommées ici .got et .got.plt font en réalité partie d’une table, que l’on nomme la GOT, pour Global Offset Table.

# GOT
```nasm
(gdb) x/12x 0x08049818
0x8049818 <_GLOBAL_OFFSET_TABLE_>:  0x0804974c      0xb7fff918      0xb7ff26a0      0x08048386
0x8049828 <_exit@got.plt>:          0x08048396      0x080483a6      0x080483b6      0x080483c6
0x8049838 <exit@got.plt>:           0x080483d6      0xb7e453e0      0x00000000      0x00000000
```

- Comme nous venons de le voir, les entrées de la PLT référencent des pointeurs situés dans la GOT. Pour comprendre le rôle de ces deux tables, regardons comment se déroule un appel a exit() : 

```nasm
0x080483d0 <exit@plt+0>:     jmp    *0x8049838
0x080483d6 <exit@plt+6>:     push   $0x28
0x080483db <exit@plt+11>:    jmp    0x8048370
```

Le 1er jmp référence un pointeur (0x8049838) situé dans la GOT. Voyons ce qu’il contient :
```
x *0x8049838
0x80483d6 <exit@plt+6>: 0x00002868
```

- qui correspond… à une adresse de la PLT, et plus précisément à l’adresse de l’instruction juste après le jmp.
- En d’autres termes, lorsque exit() sera appelée, on saute dans la PLT, et on exécute le 1er jmp, qui se contente de sauter sur l’instruction suivante.

- Continuons le fil de l’exécution : après ce 1er jmp, on rencontre un push, qui empile une certaine valeur, ici 0×28. Enfin, on rencontre un jmp, et en examinant l’adresse de saut on s’aperçoit qu’il nous emmène sur l’entrée 0 de la PLT.

- Une fois sur l’entrée 0, on rencontre deux instructions (les deux suivantes ne sont pas vraiment des instructions, mais correspondent aux opcodes 0×0000) :

```nasm
0x08048370:  pushl  0x804981c
0x08048376:  jmp    *0x8049820

(gdb) x/x 0x804981c
0x804981c <_GLOBAL_OFFSET_TABLE_+4>:    0xb7fff918
(gdb) x/x 0x8049820
0x8049820 <_GLOBAL_OFFSET_TABLE_+8>:    0xb7ff26a0
```

- On commence par empiler une valeur, qui se révèle être l’entrée 1 de la GOT. On effectue ensuite un jmp sur le contenu d’un pointeur, se situant lui aussi dans la GOT mais à l’entrée 2. Pour le moment, ce pointeur ne contient que des zéros, vu que l’exécution n’a pas encore commencée. Plaçons un breakpoint sur l’appel à exit(), et examinons la valeur du pointeur : 

```nasm
(gdb) b *n+61
(gdb) r
(gdb) x/x 0x8049820
0x8049820 <_GLOBAL_OFFSET_TABLE_+8>:    0xb7ff26a0

(gdb) info file
0xb7fde820 - 0xb7ff6baf is .text in /lib/ld-linux.so.2

```
La région contenant cette adresse d’est rien d’autre que la .text du linker dynamique, ld.so ! On se trouve dans la fonction du linker permettant d’appeler la véritable fonction exit().
