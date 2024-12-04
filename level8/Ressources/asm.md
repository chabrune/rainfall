# level8 - Analyse Assembleur Détaillée

## Fonction Main

**Initialisation**
```nasm
   0x08048564 <+0>:     push   %ebp
   0x08048565 <+1>:     mov    %esp,%ebp
   0x08048567 <+3>:     push   %edi
   0x08048568 <+4>:     push   %esi
   0x08048569 <+5>:     and    $0xfffffff0,%esp
   0x0804856c <+8>:     sub    $0xa0,%esp
```
Prologue standard et allocation de 160 bytes sur la stack

**Boucle Principale**
```nasm
   0x08048575 <+17>:    mov    0x8049ab0,%ecx
   0x0804857b <+23>:    mov    0x8049aac,%edx
   0x08048581 <+29>:    mov    $0x8048810,%eax
   0x08048591 <+45>:    call   0x8048410 <printf@plt>
```
Affiche l'état des variables globales

**Lecture de l'Entrée**
```nasm
   0x080485a7 <+67>:    lea    0x20(%esp),%eax
   0x080485ab <+71>:    mov    %eax,(%esp)
   0x080485ae <+74>:    call   0x8048440 <fgets@plt>
```
Lecture de 128 bytes (0x80) depuis stdin

**Commande Auth**
```nasm
   0x080485e4 <+128>:   movl   $0x4,(%esp)
   0x080485eb <+135>:   call   0x8048470 <malloc@plt>
   0x080485f0 <+140>:   mov    %eax,0x8049aac
```
Alloue 4 bytes et stocke le pointeur dans une variable globale

**Commande Reset**
```nasm
   0x0804866b <+263>:   mov    0x8049aac,%eax
   0x08048670 <+268>:   mov    %eax,(%esp)
   0x08048673 <+271>:   call   0x8048420 <free@plt>
```
Libère la mémoire allouée

**Commande Service**
```nasm
   0x080486a1 <+317>:   lea    0x20(%esp),%eax
   0x080486a8 <+324>:   mov    %eax,(%esp)
   0x080486ab <+327>:   call   0x8048430 <strdup@plt>
   0x080486b0 <+332>:   mov    %eax,0x8049ab0
```
Duplique l'entrée dans une nouvelle allocation

**Commande Login**
```nasm
   0x080486e2 <+382>:   mov    0x8049aac,%eax
   0x080486e7 <+387>:   mov    0x20(%eax),%eax
   0x080486ea <+390>:   test   %eax,%eax
   0x080486ee <+394>:   movl   $0x8048833,(%esp)
   0x080486f5 <+401>:   call   0x8048480 <system@plt>
```
Vérifie une condition à auth+32 et exécute system() si vraie

