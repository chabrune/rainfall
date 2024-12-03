# level0 - Analyse Assembleur Détaillée

## Fonction Main

```nasm
   0x08048ec0 <+0>:     push   %ebp
   0x08048ec1 <+1>:     mov    %esp,%ebp
   0x08048ec3 <+3>:     and    $0xfffffff0,%esp
```
Prologue standard: sauvegarde du frame pointer et alignement de la stack

```nasm
   0x08048ec6 <+6>:     sub    $0x20,%esp
```
Allocation de 32 bytes sur la stack pour les variables locales

```nasm
   0x08048ec9 <+9>:     mov    0xc(%ebp),%eax
   0x08048ecc <+12>:    add    $0x4,%eax
   0x08048ecf <+15>:    mov    (%eax),%eax
```
Récupération de argv:
- ebp+12 contient argv
- +4 pour accéder au premier argument
- Déréférencement pour obtenir la valeur

```nasm
   0x08048ed1 <+17>:    mov    %eax,(%esp)
   0x08048ed4 <+20>:    call   0x8049710 <atoi>
```
Préparation et appel de atoi() avec argv

```nasm
   0x08048ed9 <+25>:    cmp    $0x1a7,%eax
   0x08048ede <+30>:    jne    0x8048f58 <main+152>
```
Comparaison du résultat avec 423 (0x1a7)
Si différent, saut vers la fin du programme

```nasm
   0x08048ee0 <+32>:    movl   $0x80c5348,(%esp)
   0x08048ee7 <+39>:    call   0x8050bf0 <strdup>
```
Duplique la chaîne "/bin/sh"

```nasm
   0x08048ef8 <+56>:    call   0x8054680 <getegid>
   0x08048efd <+61>:    mov    %eax,0x1c(%esp)
   0x08048f01 <+65>:    call   0x8054670 <geteuid>
   0x08048f06 <+70>:    mov    %eax,0x18(%esp)
```
Récupération des IDs effectifs de groupe et utilisateur

```nasm
   0x08048f21 <+97>:    call   0x8054700 <setresgid>
   0x08048f3d <+125>:   call   0x8054690 <setresuid>
```
Configuration des droits d'accès

```nasm
   0x08048f51 <+145>:   call   0x8054640 <execv>
```
Exécution de "/bin/sh" avec les nouveaux droits

```nasm
   0x08048f7b <+187>:   call   0x804a230 <fwrite>
```
Si la comparaison échoue, affiche un message d'erreur

```nasm
   0x08048f85 <+197>:   leave  
   0x08048f86 <+198>:   ret
```
Épilogue standard: restauration du contexte et retour
