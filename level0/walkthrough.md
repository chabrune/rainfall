# level0

On lance le programme avec gdb

On regarde les jump et les cmp qui permettent de faire des conditions au programme
```
On voit :
   0x08048ed4 <+20>:    call   0x8049710 <atoi>
   0x08048ed9 <+25>:    cmp    $0x1a7,%eax
   0x08048ede <+30>:    jne    0x8048f58 <main+152>
```

On comprend que le programme fait un atoi sur le parametre passe au programme car sinon fait un segmentation fault

Ensuite comparaison entre 423 (0x1a7) et le registre eax (le retour de atoi)

On envoie 423 en parametre au programme

`0x08048ede <+30>:    jne    0x8048f58 <main+152>`

la comparaison sera egale et il n'y aura pas de jump donc les instructions suivantes seront executees

A savoir un execve("/bin/sh") qui lancera le shell level1 avec le bit SUID

`-rwsr-x---+ 1 level1 users  747441 Mar  6  2016 level0*`

```
$ whoami
level1
```

On est bien passe level1 donc maintenant reste plus qu'a lire le fichier .pass sur le home du level1

`$ cat /home/user/level1/.pass`
