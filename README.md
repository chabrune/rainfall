# 🔐 Rainfall - Exploitation de Binaires & Reverse Engineering

## Introduction
Rainfall est un projet d'exploitation binaire qui m'a permis d'approfondir mes connaissances en sécurité informatique à travers une série de challenges progressifs. Chaque niveau présente une vulnérabilité différente à exploiter pour obtenir le mot de passe du niveau suivant.

## Compétences Techniques Développées

### Architecture Mémoire
- Compréhension approfondie de la stack et heap
- Manipulation des registres (EIP, ESP, EBP...)
- Analyse des segments mémoire (BSS, Data, Text...)
- Protection binaire (ASLR, NX, Stack Canary...)

### Techniques d'Exploitation
| Niveau | Vulnérabilité | Technique Utilisée |
|--------|---------------|-------------------|
| level0 | Integer Comparison | Exploitation d'atoi() pour obtenir un shell |
| level1 | Buffer Overflow | Stack Buffer Overflow |
| level2 | Buffer Overflow | Execution d'un shellcode dans la Heap via strdup |
| level3 | Format String | Modification de variable globale |
| level4 | Format String | Réécriture mémoire avec printf |
| level5 | Format String | Détournement de GOT |
| level6 | Function Pointer | Écrasement de pointeur de fonction |
| level7 | GOT Overwrite | Détournement de fonction libc |
| level8 | Trolling | Exploitation de la heap |
| level9 | C++ vtable | Exploitation d'objets C++ |
| bonus0 | Buffer Overflow | Detournement flux via EIP |
| bonus1 | Integer Comparison | Integer Overflow |
| bonus2 | ret2libc | Execution bin/sh via libc |
| bonus3 | TROLLING | TROLLING |


### Méthodologie
1. Analyse du binaire - Reverse
2. Identification des vulnérabilités
3. Développement de l'exploit
4. Documentation

---
*Projet réalisé dans le cadre du cursus de l'école 42*
