```nasm
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/user/levelX/levelX
```

## RELRO (Relocation Read-Only)
**No RELRO** signifie que la Global Offset Table (GOT) reste modifiable pendant l'exécution. Cela rend le binaire vulnérable aux attaques de réécriture de la GOT. Il existe deux niveaux de protection :
- Partial RELRO : Seule la GOT est en lecture seule
- Full RELRO : La GOT et la PLT sont en lecture seule

## Stack Canary
**No canary found** indique l'absence de protection contre les débordements de buffer sur la pile. Les canaris sont des valeurs placées entre les variables locales et les données de contrôle. Si cette valeur est modifiée, le programme s'arrête.

## NX (No-eXecute)
**NX disabled** signifie que la pile est exécutable. Cela permet l'exécution de code injecté dans la pile, facilitant les exploits de type buffer overflow.

## PIE (Position Independent Executable)
**No PIE** indique que le binaire n'est pas compilé en tant qu'exécutable à position indépendante. Les adresses sont fixes et non randomisées à chaque exécution, facilitant les attaques.

## RPATH/RUNPATH
**No RPATH/No RUNPATH** signifie qu'aucun chemin de recherche personnalisé n'est défini pour les bibliothèques partagées. C'est généralement préférable pour la sécurité car ces options peuvent être exploitées pour charger des bibliothèques malveillantes.

## ASLR (Address Space Layout Randomization)

L'ASLR est une technique de sécurité qui randomise l'emplacement des zones mémoire clés d'un processus, notamment :
- La base de l'exécutable
- La pile (stack)
- Le tas (heap)
- Les bibliothèques partagées