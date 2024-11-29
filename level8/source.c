int main() {
    char buf[128];
    while (1) {
        printf("%p, %p\n", auth, service);
        if (!fgets(buf, 128, stdin)) break;

        if (strncmp(buf, "auth ", 5) == 0) {
            auth = malloc(4);
            *auth = 0;
            if (strlen(buf + 5) <= 30)
                strcpy(auth, buf + 5);
        }
        else if (strncmp(buf, "reset", 5) == 0) {
            free(auth);
        }
        else if (strncmp(buf, "service", 7) == 0) {
            service = strdup(buf + 7);
        }
        else if (strncmp(buf, "login", 5) == 0) {
            if (*(auth + 32) != 0) {
                system("/bin/sh");
            } else {
                printf("Password:\n");
            }
        }
    }
    return 0;
}
