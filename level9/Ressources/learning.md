La vtable (Virtual Table) est un mécanisme fondamental en C++ qui permet l'implémentation du polymorphisme dynamique.

## Structure et Fonctionnement

**Composition de la vtable**
- Une table contenant des pointeurs vers les fonctions virtuelles d'une classe
- Créée automatiquement par le compilateur pour chaque classe contenant des fonctions virtuelles
- Partagée entre tous les objets d'une même classe

**vptr (Virtual Pointer)**
- Pointeur caché ajouté automatiquement comme membre de la classe
- Pointe vers la vtable de la classe correspondante
- Ajouté uniquement si la classe contient ou hérite de fonctions virtuelles

## Mécanisme de Résolution

**Fonctionnement**
1. Lors de la création d'un objet, le vptr est initialisé pour pointer vers la vtable de sa classe
2. À l'appel d'une fonction virtuelle, le système :
   - Accède à la vtable via le vptr de l'objet
   - Trouve l'adresse de la fonction appropriée dans la table
   - Exécute la fonction correspondante

**Organisation Mémoire**
- Chaque fonction virtuelle possède une entrée unique dans la vtable
- Les classes dérivées héritent du vptr de la classe de base
- La vtable contient également des informations pour l'identification du type à l'exécution (RTTI)

Ce mécanisme permet au C++ d'implémenter l'appel de la bonne version d'une fonction virtuelle lors de l'exécution, permettant ainsi le polymorphisme dynamique
