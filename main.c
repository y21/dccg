#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_BUFF 64
#define PREFIX_BUFF 4
#define PRESENCE_TYPE_BUFF 10
#define PRESENCE_NAME_BUFF 48
#define SNOWFLAKE_LEN 20
#define IGNORED_SERVERS_LEN 8
#define COMMAND_NAME_LENGTH 12
#define DCCG_VER "1.1"

enum presence_type {
    PLAYING,
    WATCHING,
    LISTENING,
    NONE
};

struct presence {
    enum presence_type type;
    char* name;
};

struct server {
    char id[SNOWFLAKE_LEN];
    char verificationChannel[SNOWFLAKE_LEN];
    char verifyRole[SNOWFLAKE_LEN];
};

struct command {
    char name[COMMAND_NAME_LENGTH];
    char** executors;
    char** requiredPermissions;
    int enabled;
};

// Config data
char* token;
char* prefix;
int delete_messages;
struct presence prs;
int serverCount;
struct server* servers;
int serverIgnoreCount;
char** ignoredServers;
struct command commands[] = {
    { "ping" },
    { "verify" },
    { "unverify" },
    { "version" }
};

void get_input(char* str, int length) {
    int n = 0;
    while(1) {
        char temp = getchar();
        if (temp == '\n') {
            if (n > 0) break;
        }
        if (n >= length) {
            str = (char*) realloc(str, sizeof(char) * (length *= 2));
        }
        if (temp != '\n') {
            str[n] = temp;
            n++;
        }
    }
    str[n] = '\0';
}

void save(FILE* handle) {
    fprintf(handle, "{\n\t\"token\": \"%s\"\n}", token);
}

int main() {
    char temp;
    token = (char*) malloc(sizeof(char) * TOKEN_BUFF);
    prefix = (char*) malloc(sizeof(char) * PREFIX_BUFF);
    prs.name = (char*) malloc(sizeof(char) * PRESENCE_NAME_BUFF);
    char* prsRawType = (char*) malloc(sizeof(char) * PRESENCE_TYPE_BUFF);
    FILE* fd = fopen("config.json", "w+");

    // General
    printf("====== DiscordCaptcha Config File Generator %s ======\n- General setup\nBot Token: ", DCCG_VER);
    get_input(token, TOKEN_BUFF);
    printf("Prefix: ");
    get_input(prefix, PREFIX_BUFF);
    printf("Delete Messages (y/n): ");
    if ((temp = getchar()) != 'y' && temp != 'n') {
        fprintf(stderr, "%s", "\"Delete Messages\" expects 'y' or 'n'.");
    } else {
        delete_messages = temp == 'y';
    }
    // Presence
    printf("- Presence\nPresence type (playing / watching / listening - leave blank if no presence): ");
    get_input(prsRawType, PRESENCE_TYPE_BUFF);
    if (strcmp(prsRawType, "playing") == 0)
        prs.type = PLAYING;
    else if (strcmp(prsRawType, "watching") == 0)
        prs.type = WATCHING;
    else if (strcmp(prsRawType, "listening") == 0)
        prs.type = LISTENING;
    else 
        prs.type = NONE;

    if (prs.type != NONE) {
        printf("Presence name (playing status): ");
        get_input(prs.name, PRESENCE_TYPE_BUFF);
    }

    // Servers
    printf("- Servers\nOn how many servers is this bot going to be? (-1 to skip): ");
    scanf("%d", &serverCount);
    if (serverCount != -1) {
        servers = (struct server*) malloc(sizeof(struct server) * serverCount);
        for (int i = 0; i < serverCount; ++i) {
            printf("[%d] Server ID: ", i + 1);
            get_input(servers[i].id, SNOWFLAKE_LEN);
            printf("[%d] Verification Channel ID: ", i + 1);
            get_input(servers[i].verificationChannel, SNOWFLAKE_LEN);
            printf("[%d] Verification Role ID: ", i + 1);
            get_input(servers[i].verifyRole, SNOWFLAKE_LEN);
        }
    }

    // Ignore servers
    printf("- Ignore servers\nHow many servers should be ignored? (-1 to skip): ");
    scanf("%d", &serverIgnoreCount);
    if (serverIgnoreCount != -1) {
        ignoredServers = (char**) malloc(serverIgnoreCount * SNOWFLAKE_LEN);
        for (int i = 0; i < serverIgnoreCount; ++i) {
            printf("[%d] Enter Server ID to ignore: ", i + 1);
            get_input(ignoredServers[i], SNOWFLAKE_LEN);
        }
    }

    // Commands
    for (int i = 0; i < sizeof(commands) / sizeof(struct command); ++i) {

    }


    // Write to file and close
    save(fd);
    fclose(fd);

    // Free memory
    free(token);
    free(prefix);
    free(prs.name);
    free(prsRawType);
    free(servers);
    free(ignoredServers);
}
