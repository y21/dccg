#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_BUFF 64
#define PREFIX_BUFF 4

char* get_input(char* str, int length) {
    int n = 0;
    char temp;
    while((temp = getchar()) != '\n') {
        if (n >= length) {
            str = (char*) realloc(str, sizeof(char) * (length *= 2));
        }
        str[n] = temp;
        n++;
    }
    str[n] = '\0';
}

int main() {
    const char version[] = "v1.1";

    char* token = (char*) malloc(sizeof(char) * TOKEN_BUFF);
    char* prefix = (char*) malloc(sizeof(char) * PREFIX_BUFF);

    printf("====== DiscordCaptcha Config File Generator %s ======\nBot Token: ", version);
    get_input(token, TOKEN_BUFF);
    printf("Prefix: ");
    get_input(prefix, PREFIX_BUFF);

    free(token);
    free(prefix);
}
