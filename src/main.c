#include <stdio.h>
#include <string.h>

// Constants
#define VERSION "1.2"
#define MAX_TOKEN_LEN 64
#define MAX_PREFIX_LEN 8
#define MAX_PRESENCE_TYPE_LEN 16
#define MAX_PRESENCE_NAME_LEN 32
#define MAX_SNOWFLAKE_LEN 20
#define MAX_IGNORED_SERVERS MAX_PREFIX_LEN
#define MAX_COMMAND_NAME_LEN 24
#define MAX_EXECUTORS MAX_PREFIX_LEN
#define MAX_PERMISSIONS MAX_PREFIX_LEN
#define MAX_PERMISSION_LEN MAX_COMMAND_NAME_LEN
#define MAX_GUILD_LEN 16

// Structs
typedef struct {
	char type[MAX_PRESENCE_TYPE_LEN];
	char name[MAX_PRESENCE_NAME_LEN];
} presence;

typedef struct {
	char id[MAX_SNOWFLAKE_LEN];
	char verification_channel[MAX_SNOWFLAKE_LEN];
	char verification_role[MAX_SNOWFLAKE_LEN];
	int valid;
} guild;

typedef struct {
	char command_name[MAX_COMMAND_NAME_LEN];
	int executors_count;
	char executors[MAX_EXECUTORS][MAX_SNOWFLAKE_LEN];
	int permissions_count;
	char required_permissions[MAX_PERMISSIONS][MAX_PERMISSION_LEN];
	int enabled;
} command;

// Data
char token[MAX_TOKEN_LEN];
char prefix[MAX_PREFIX_LEN];
int delete_messages;
presence prs;
guild servers[MAX_GUILD_LEN];
int guild_count;
int ignored_servers[MAX_IGNORED_SERVERS];
int guild_ignore_count;
command commands[] = {
	{ "ping", {}, {}, 1 },
	{ "verify", {}, {}, 1 }
};



int validate_snowflake(char snf[MAX_SNOWFLAKE_LEN]) {
	for(int i = 0; i < strlen(snf); ++i) {
		if (snf[i] < 48 || snf[i] > 57) {
			return 0;
		}
	}
	return strlen(snf) > 15;
}

// Main
int main() {
	printf(
		"--- Discordcaptcha Config File Generator %s ---\n"
		"1    Config\n"
		"2    Exit\n"
	, VERSION);
	int input;
	scanf("%d", &input);
	getchar(); // \n in buffer

	switch(input) {
		case 1: {
			FILE* fd = fopen("config.json", "w+");
			fprintf(fd, "{}");

			printf("Enter Bot token: ");
			fgets(token, sizeof(token), stdin);

			printf("Enter prefix: ");
			fgets(prefix, sizeof(prefix), stdin);

			printf("Delete messages (1 = yes, 0 = no): ");
			scanf("%d", &delete_messages);
			getchar(); // \n
			if (delete_messages > 1 || delete_messages < 0) {
				fprintf(stderr, "[x] delete_messages must have a value between 0 and 1.");
				return 1;
			}

			printf("Presence type (leave blank if no presence): ");
			fgets(prs.type, sizeof(prs.type), stdin);

			if (!strcmp(prs.name, "")) {
				printf("Presence name: ");
				fgets(prs.name, sizeof(prs.name), stdin);
			}

			printf("To how many servers should discordcaptcha listen to (1-16): ");
			scanf("%d", &guild_count);
			getchar();

			if (guild_count > 16 || guild_count < 1) {
				fprintf(stderr, "[x] guild_count must have a value between 1 and 16.");
				return 1;
			}

			for(int i = 0; i < guild_count; ++i) {
				printf("  [G%d] Guild ID: ", i + 1);
				fgets(servers[i].id, sizeof(servers[i].id), stdin);

				printf("  [G%d] Verification Channel ID: ", i + 1);
				fgets(servers[i].verification_channel, sizeof(servers[i].verification_channel), stdin);

				printf("  [G%d] Verification Role ID: ", i + 1);
				fgets(servers[i].verification_role, sizeof(servers[i].verification_role), stdin);

				if (!validate_snowflake(servers[i].id)) {
					printf("  [x] [G%d] Invalid Guild ID\n", i + 1);
					servers[i].valid = 0;
				} else if (!validate_snowflake(servers[i].verification_channel)) {
					printf("  [x] [G%d] Invalid Verification Channel ID\n", i + 1);
					servers[i].valid = 0;
				} else if (!validate_snowflake(servers[i].verification_role)) {
					printf("  [x] [G%d] Invalid Verification Role ID\n", i + 1);
					servers[i].valid = 0;
				} else {
					servers[i].valid = 1;
				}
			}
			
			printf("How many servers should be ignored (0-8): ");
			scanf("%d", &guild_ignore_count);
			getchar();
			if (guild_ignore_count > 8 || guild_ignore_count < 0) {
				fprintf(stderr, "[x] guild_ignore_count must have a value between 0 and 8.");
				return 1;
			}
			// TODO: ask for ignored servers

			for (int i = 0; i < sizeof(commands) / sizeof(command); ++i) {
				printf("  [C%d] Enable command %s (1 = yes, 0 = no): ", i + 1, commands[i].command_name);
				scanf("%d", &commands[i].enabled);
				getchar();
				if (commands[i].enabled > 1 || commands[i].enabled < 0) {
					fprintf(stderr, "  [x] commands[%d] (~%s).enabled must have a value between 0 and 1.", i, commands[i].command_name);
					return 1;
				}
				printf("    [C%d][1] How many users should be allowed to execute this command (0 = everyone): ", i + 1);
				scanf("%d", &commands[i].executors_count);
				for (int j = 0; j < commands[i].executors_count; ++j) {
					printf(""); // TODO: ask for user ids
				}
			}

		}
		break;
		case 2:
			return 0;
		break;
	}
}
