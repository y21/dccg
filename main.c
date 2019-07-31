#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TOKEN_BUFF 64
#define PREFIX_BUFF 4
#define PRESENCE_TYPE_BUFF 10
#define PRESENCE_NAME_BUFF 48
#define SNOWFLAKE_LEN 20
#define IGNORED_SERVERS_LEN 8
#define COMMAND_NAME_LENGTH 12
#define MAX_PERMISSION_LENGTH 28
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
    int executorCount;
    int permissionCount;
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

void get_input_c(char c) {
    int n = 0;
    while(1) {
        char temp = getchar();
        if (temp == '\n') {
            if (n > 0) break;
        } else {
            c = temp;
            n++;
        }
    }
}

void save(FILE* handle) {
    fprintf(handle,
        "{\n\t\"token\": \"%s\""
    , token);
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
    printf("- Commands\n");
    for (int i = 0; i < sizeof(commands) / sizeof(struct command); ++i) {
        struct command cmd = commands[i];
        char enableCmd;
        printf("\nEnable command \"%s\"? (y/n): ", cmd.name);
        scanf(" %c", &enableCmd);
        if (enableCmd != 'y' && enableCmd != 'n') {
            printf("[!] 'y' or 'n' expected.\n");
            i--;
            continue;
        } else {
            cmd.enabled = enableCmd == 'y';
        }

        if (cmd.enabled == 0) continue;

        printf("Total command executors (-1 to allow everyone to execute this command): ");
        scanf("%d", &cmd.executorCount);
        if (cmd.executorCount != -1) {
            cmd.executors = (char**) malloc(cmd.executorCount * SNOWFLAKE_LEN); // TODO: free() this later
            for (int j = 0; j < cmd.executorCount; ++j) {
                printf("[%s] Enter User ID: ", cmd.name);
                get_input(cmd.executors[j], SNOWFLAKE_LEN);
            }
        }

        printf("Required permissions to execute command (-1 for no permissions): ");
        scanf("%d", &cmd.permissionCount);
        if (cmd.permissionCount != -1) {
            cmd.requiredPermissions = (char**) malloc(cmd.permissionCount * MAX_PERMISSION_LENGTH); // TODO: free() this later
            for (int j = 0; j < cmd.permissionCount; ++j) {
                printf("[%s] Enter Permission: ", cmd.name);
                get_input(cmd.requiredPermissions[j], MAX_PERMISSION_LENGTH);
                for (int k = 0; k < strlen(cmd.requiredPermissions[j]); ++k) {
                    cmd.requiredPermissions[j][k] = toupper(cmd.requiredPermissions[j][k]);
                }
            }
        }

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
