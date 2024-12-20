Site incroyable sur PLT/GOT/Linkage/Lib 
`https://www.segmentationfault.fr/linux/role-plt-got-ld-so/`


# Resume & Exemple concret avec binaire level5

- Quand on compile un programme qui fait appel à des fonctions situées dans d’autres bibliothèques (telles que la librairie standard), l’édition de liens peut être faite de deux façons différentes. La première méthode, dite statique, consiste à intégrer à l’exécutable toutes les librairies dont il a besoin pour fonctionner. A l’exécution, tous les symboles sont donc résolus, et les appels sont immédiats.
- Depuis les versions « récentes » de Linux, c’est la deuxième méthode d’édition de liens, dite dynamique, qui est utilisée par défaut. Avec cette méthode, chaque librairie est compilée une fois pour toute dans une librairie dynamique, ou partagée (shared) ayant l’extension .so (équivalent des .dll sous Windows). Lorsque l’on compile un programme qui y fait référence, on y insère juste le nom du symbole (fonction ou variable) dont il a besoin, ainsi que le nom de la librairie. C’est à l’exécution du programme que l’éditeur de liens dynamique (ou dynamic linker), nommé ld.so, charge les libraires nécessaire et effectue la résolution des symboles manquants en temps réel.
- Enfin, il convient de clarifier la notion de PIC, ou Position Independant Code. Un code exécutable est dit PIC s’il peut être mappé à l’importe quelle région mémoire tout en pouvant s’exécuter convenablement. Dans de tels exécutables, aucune adresse absolue ne doit apparaître, puisque si l’exécutable se retrouve translaté en mémoire, les adresses absolues ne seront plus valides. Dans Linux, les librairies dynamiques sont en PIC. C’est le linker dynamique, ld.so, qui les charge en mémoire à l’exécution, et leur place en mémoire peut varier d’une exécution à une autre. Ainsi, l’adresse des fonctions de la libraire standard, telles que printf(), changent de place à chaque exécution. Pourtant, un programme qui utilise printf() n’est compilé qu’une seule fois. Comment les processus arrivent-ils donc à s’exécuter tout en prenant en compte cette variation d’adresses ? C’est là tout l’objectif de cet article…

```nasm
(gdb) info files
0x08048370 - 0x080483f0 is .plt
0x08049814 - 0x08049818 is .got
0x08049818 - 0x08049840 is .got.plt
```

## PLT
```nasm
(gdb) disas 0x08048370, 0x080483f0
Dump of assembler code from 0x8048370 to 0x80483f0:
   0x08048370:  pushl  0x804981c  ---> 0x804981c <_GLOBAL_OFFSET_TABLE_+4>:    0xb7fff918
   0x08048376:  jmp    *0x8049820 ---> 0x8049820 <_GLOBAL_OFFSET_TABLE_+8>:    0xb7ff26a0
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

## GOT
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
- La région contenant cette adresse d’est rien d’autre que la .text du linker dynamique, ld.so ! On se trouve dans la fonction du linker permettant d’appeler la véritable fonction exit().

## La résolution des symboles

- Mais comment le linker sait-il qu’il faut appeler exit() et pas une autre fonction ? Tout simplement grâce au push 0×28 précédent : 0×28 correspond en fait à un offset correspondant ici à exit(). Les instructions de la PLT l’ont empilé afin de le passer en paramètre à la fonction du linker.

Désassemblons cette fonction :
```nasm
(gdb) disas 0xb7ff26a0, 0xb7ff26a0+28
Dump of assembler code from 0xb7ff26a0 to 0xb7ff26bc:
   0xb7ff26a0:  push   %eax
   0xb7ff26a1:  push   %ecx
   0xb7ff26a2:  push   %edx
   0xb7ff26a3:  mov    0x10(%esp),%edx
   0xb7ff26a7:  mov    0xc(%esp),%eax
   0xb7ff26ab:  call   0xb7fec1d0
   0xb7ff26b0:  pop    %edx
   0xb7ff26b1:  mov    (%esp),%ecx
   0xb7ff26b4:  mov    %eax,(%esp)
   0xb7ff26b7:  mov    0x4(%esp),%eax
   0xb7ff26bb:  ret    $0xc
```

- Cette fonction est relativement courte ; elle ne fait en réalité qu’appeler la véritable fonction de résolution des adresses. Lorsqu’elle est appelée, le sommet de pile contient une adresse (correspondant à GOT[1], qui a été pushée à l’entrée 0 de la PLT), et juste en dessous se trouve l’index de exit, soit 0x28. Vérifions en plaçant un breakpoint au début et en examinant la pile :

```nasm
(gdb) b *0xb7ff26a0
Breakpoint 2 at 0xb7ff26a0

(gdb) c
Continuing.

(gdb) x/2x $esp
0xbffff414:     0xb7fff918      0x00000028
```

Vérifions que le 1er pointeur correspond bien à l’entrée 1 de la GOT :

```nasm
(gdb) x/x 0x804981c
0x804981c <_GLOBAL_OFFSET_TABLE_+4>:    0xb7fff918
```

Revenons au code de cette fonction :

```nasm
(gdb) disas 0xb7ff26a0, 0xb7ff26a0+28
Dump of assembler code from 0xb7ff26a0 to 0xb7ff26bc:
   0xb7ff26a0:  push   %eax
   0xb7ff26a1:  push   %ecx
   0xb7ff26a2:  push   %edx
   0xb7ff26a3:  mov    0x10(%esp),%edx
   0xb7ff26a7:  mov    0xc(%esp),%eax
   0xb7ff26ab:  call   0xb7fec1d0
   0xb7ff26b0:  pop    %edx
   0xb7ff26b1:  mov    (%esp),%ecx
   0xb7ff26b4:  mov    %eax,(%esp)
   0xb7ff26b7:  mov    0x4(%esp),%eax
   0xb7ff26bb:  ret    $0xc
```

- Elle commence par 3 push, permettant de sauvegarder des registres. Ainsi, nos deux valeurs en sommet de pile vont être décalées de 3*4 = 12 octets. Juste après ces 3 push, on a deux mov. Le premier place dans %edx une valeur située sur la pile à l’offset 0×10 soit 16 = 4 * 4 octets. Il s’agit donc de l’index de exit(), 0×28 : `edx            0x28`. Le second place dans %eax la valeur suivante, soit celle de GOT[1] : `eax            0xb7fff918`. Puis un appel de fonction a lieu.

- On arrive alors dans une fonction relativement complexe, qui se situe toujours dans la section .text de ld.so. C’est elle qui est chargée d’effectuer la résolution des symboles en recherchant dans les librairies.

- Continuons donc. Plaçons un breakpoint juste après le call de cette fonction, en 0xb7ff26b0

```nasm
(gdb) b *0xb7ff26b0

(gdb) c
Continuing.

Breakpoint 3, 0xb7ff26b0 in ?? () from /lib/ld-linux.so.2

(gdb) x/4i $pc
=> 0xb7ff26b1:  mov    (%esp),%ecx
   0xb7ff26b4:  mov    %eax,(%esp)
   0xb7ff26b7:  mov    0x4(%esp),%eax
   0xb7ff26bb:  ret    $0xc
```

- Les instructions suivantes manipulent des registres. Le mov place la valeur de %eax en sommet de pile, tandis que le ret ordonne au CPU de continuer l’exécution du code à l’adresse située sur le sommet de pile. Autrement dit, juste après le call de la fonction de résolution des symboles, on saute sur l’adresse contenue dans %eax ! Regardons ce que vaut ce registre…

- eax            0xb7e5ebe0

0xb7e42f10 - 0xb7f7736c is .text in /lib/i386-linux-gnu/libc.so.6
Tiens, elle se trouve dans la .text… Par hasard, ce ne serait pas l’adresse de exit ?

```nasm
(gdb) x 0xb7e5ebe0
0xb7e5ebe0 <exit>
(gdb) p exit
$4 = {<text variable, no debug info>} 0xb7e5ebe0 <exit>
```

Eh si ! Autrement dit, la fonction de résolution des symboles a résolu correctement exit et a placé son adresse dans %eax.

- Comme on a pu le constater, chaque appel de fonction entraîne à priori une résolution de symbole, ce qui paraît fastidieux. Fort heureusement, par défaut, ld.so ne résoud pas un symbole à chaque fois qu’on tente d’y accéder, mais uniquement la 1ère fois. Par exemple, si vous avez 10 appels à exit() dans un programme, le 1er appel entraînera une résolution, et l’adresse de exit() sera gardée en mémoire pour les 9 appels suivant. C’est ce que l’on appelle l’évaluation fainéante : on ne fait que le minimum d’opération, et juste à temps.

- Où et comment les adresses des symboles sont-elles gardées en mémoire une fois résolues ? Réponse : dans la GOT ! Pour le comprendre, relançons le programme et plaçons un breakpoint dans l’entrée de la PLT correspondant à exit.


## Recapitulatif

- 1er appel de exit : symbole non encore résolu

```nasm
0x80484ff <n+61>:    call   0x80483d0 <exit@plt> ---
                                                   |
                                                   |
                                                   |
                    <------------------------------+
<exit@plt>

0x80483d0 <exit@plt>:        jmp    *0x8049838 ---------------> 0x8049838 (<exit@got.plt>) --
0x80483d6 <exit@plt+6>:      push   $0x28       <-------------------------------------------|

0x80483db <exit@plt+11>:     jmp    0x8048370 ---------------
                                                            |
                                                            |
                               <----------------------------+
Entree 0  de la PLT
   |
   |
   |
0x8048370:   pushl  0x804981c          -----------> 0x804981c <_GLOBAL_OFFSET_TABLE_+4>:    0xb7fff918
0x8048376:   jmp    *0x8049820         -----------> 0x8049820 <_GLOBAL_OFFSET_TABLE_+8>:    0xb7ff26a0
                        |
                        |
      <-----------------+ is .text in /lib/ld-linux.so.2
0xb7ff26a0:  push   %eax
0xb7ff26a1:  push   %ecx
0xb7ff26a2:  push   %edx
0xb7ff26a3:  mov    0x10(%esp),%edx    -------------> edx 0x28 - Parametres passees a la fonction du linker correspondant a une fonction specifique
0xb7ff26a7:  mov    0xc(%esp),%eax
0xb7ff26ab:  call   0xb7fec1d0 -------------->  Appel de la fonction de résolution des symboles
                                                L'adresse du symbole (exit) est placé dans %eax.
                                                L'entrée de la .got est patchée avec cette adresse
                  ... <----------------------   Retour de la fonction
0xb7ff26b0:  pop    %edx
0xb7ff26b1:  mov    (%esp),%ecx
0xb7ff26b4:  mov    %eax,(%esp) --------------------> eax 0xb7e5ebe0 <exit> L'adresse de exit (0xb7e5ebe0) est empilée
0xb7ff26b7:  mov    0x4(%esp),%eax
0xb7ff26bb:  ret    $0xc         ------------------------------+  On saute sur exit
                                                               |
                                                               |
0xb7e5ebe0 in exit () from /lib/i386-linux-gnu/libc.so.6 <-----+

0xb7e5ebe0 <exit>:           push   %ebx
0xb7e5ebe1 <exit+1>:         call   0xb7f56c73
0xb7e5ebe6 <exit+6>:         add    $0x17240e,%ebx
0xb7e5ebec <exit+12>:        sub    $0x18,%esp
0xb7e5ebef <exit+15>:        movl   $0x1,0x8(%esp)
0xb7e5ebf7 <exit+23>:        lea    0x3f0(%ebx),%eax
0xb7e5ebfd <exit+29>:        mov    %eax,0x4(%esp)
0xb7e5ec01 <exit+33>:        mov    0x20(%esp),%eax
0xb7e5ec05 <exit+37>:        mov    %eax,(%esp)
0xb7e5ec08 <exit+40>:        call   0xb7e5eaf0
```

**Résolution à l'Exécution**
- Au démarrage du programme, la GOT contient initialement l'adresse de l'instruction suivante dans la PLT
- Le premier appel à une fonction suit ce chemin :
  1. Saut vers la PLT
  2. La PLT consulte la GOT qui pointe vers le code de résolution
  3. Le linker dynamique est appelé avec l'offset spécifique à la fonction
  4. L'adresse résolue est sauvegardée dans la GOT

**Après Résolution**
- Les appels suivants sont directs :
  1. Saut vers la PLT
  2. La PLT consulte la GOT qui contient maintenant l'adresse réelle
  3. Saut direct vers la fonction