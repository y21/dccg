#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_BUFF 64
#define PREFIX_BUFF 4

struct presence {
    char* type;
    char* name;
};

void get_input(char* str, int length) {
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

    char temp;
    char* token = (char*) malloc(sizeof(char) * TOKEN_BUFF);
    char* prefix = (char*) malloc(sizeof(char) * PREFIX_BUFF);
    struct presence prs;
    FILE* fd = fopen("config.json", "w+");
    int delete_messages;

    printf("====== DiscordCaptcha Config File Generator %s ======\nBot Token: ", version);
    get_input(token, TOKEN_BUFF);
    printf("Prefix: ");
    get_input(prefix, PREFIX_BUFF);
    printf("Delete Messages (y/n): ");
    if ((temp = getchar()) != 'y' && temp != 'n') {
        fprintf(stderr, "%s", "\"Delete Messages\" expects 'y' or 'n'.");
    } else {
        delete_messages = temp == 'y';
    }

    // Write to file
    fprintf(fd, "{\n\t\"token\": \"%s\"\n}", token);

    fclose(fd);
    free(token);
    free(prefix);
}
