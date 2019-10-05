#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	char permissions[MAX_PERMISSIONS][MAX_PERMISSION_LEN];
	int enabled;
} command;

// Data
char token[MAX_TOKEN_LEN];
char prefix[MAX_PREFIX_LEN];
int delete_messages;
presence prs;
guild servers[MAX_GUILD_LEN];
int guild_count;
char ignored_servers[MAX_IGNORED_SERVERS][MAX_SNOWFLAKE_LEN];
int guild_ignore_count;
command commands[] = {
	{ "ping", 0, {}, 0, {}, 1 },
	{ "verify", 0, {}, 0, {}, 1 }
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

			printf("Enter Bot token: ");
			fgets(token, sizeof(token), stdin);
			token[strcspn(token, "\n")] = 0;
			
			printf("Enter prefix: ");
			fgets(prefix, sizeof(prefix), stdin);
			prefix[strcspn(prefix, "\n")] = 0;
			
			printf("Delete messages (1 = yes, 0 = no): ");
			scanf("%d", &delete_messages);
			getchar(); // \n
			if (delete_messages > 1 || delete_messages < 0) {
				fprintf(stderr, "[x] delete_messages must have a value between 0 and 1.");
				return 1;
			}

			printf("Presence type (leave blank if no presence): ");
			fgets(prs.type, sizeof(prs.type), stdin);
			prs.type[strcspn(prs.type, "\n")] = 0;

			if (strcmp(prs.type, "")) {
				printf("Presence name: ");
				fgets(prs.name, sizeof(prs.name), stdin);
				prs.name[strcspn(prs.name, "\n")] = 0;
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
				servers[i].id[strcspn(servers[i].id, "\n")] = 0;

				printf("  [G%d] Verification Channel ID: ", i + 1);
				fgets(servers[i].verification_channel, sizeof(servers[i].verification_channel), stdin);
				servers[i].verification_channel[strcspn(servers[i].verification_channel, "\n")] = 0;

				printf("  [G%d] Verification Role ID: ", i + 1);
				fgets(servers[i].verification_role, sizeof(servers[i].verification_role), stdin);
				servers[i].verification_role[strcspn(servers[i].verification_role, "\n")] = 0;

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
			for (int i = 0; i < guild_ignore_count; ++i) {
				printf("    [IG%d] Guild ID: ", i + 1);
				fgets(ignored_servers[i], sizeof(ignored_servers[i]), stdin);
				ignored_servers[i][strcspn(ignored_servers[i], "\n")] = 0;
			}

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
				getchar();
				for (int j = 0; j < commands[i].executors_count; ++j) {
					printf("      [C%d][1] Enter User ID: ");
					fgets(commands[i].executors[j], sizeof(commands[i].executors[j]), stdin);
					commands[i].executors[j][strcspn(commands[i].executors[j], "\n")] = 0;
				}

				printf("    [C%d][2] How many permissions are required for this command (0-8): ", i + 1);
				scanf("%d", &commands[i].permissions_count);
				getchar();
				for (int j = 0; j < commands[i].permissions_count; ++j) {
					printf("      [C%d][2] Enter Permission (in UPPERCASE-letters): ");
					fgets(commands[i].permissions[j], sizeof(commands[i].permissions[j]), stdin);
					commands[i].permissions[j][strcspn(commands[i].permissions[j], "\n")] = 0;
				}
			}

			printf("\nDone! Writing to output file...");
			
			// Write to file
			char* buff = (char*)malloc(sizeof(char) * 2048); // allocate 2kb

			strcat(buff, "{\n"
						 "    \"token\": \""
			);
			strcat(buff, token);
			strcat(buff, "\",\n"
						 "    \"prefix\": \""
			);
			strcat(buff, prefix);
			strcat(buff, "\",\n"
						 "    \"deleteMessages\": "
			);
			strcat(buff, delete_messages == 1 ? "true" : "false");
			strcat(buff, ",\n"
						 "    \"presence\": "
			);
			if (!strcmp(prs.type, "")) 
				strcat(buff, "{},\n");
			else {
				strcat(buff, "{\n"
							 "        \"type\": \""
				);
				strcat(buff, prs.type);
				strcat(buff, "\",\n"
							 "        \"name\": \""
				);
				strcat(buff, prs.name);
				strcat(buff, "\"\n"
							 "    },\n"
				);
			}
			
			strcat(buff, "    \"servers\": {\n");
			for (int i = 0; i < guild_count; ++i) {
				strcat(buff, "        \"");
				strcat(buff, servers[i].id);
				strcat(buff, 
					"\": {\n"
					"            \"verificationChannel\": \""
				);
				strcat(buff, servers[i].verification_channel);
				strcat(buff, "\",\n"
							 "            \"verifyRole\": \""
				);
				strcat(buff, servers[i].verification_role);
				strcat(buff, "\"\n"
							 "        }"
				);
				strcat(buff, i != guild_count - 1 ? ",\n" : "\n");
			}
			strcat(buff, "    },\n"
						 "    \"ignoreServers\": [\n"
			);
			for (int i = 0; i < guild_ignore_count; ++i) {
				strcat(buff, "        \"");
				strcat(buff, ignored_servers[i]);
				strcat(buff, "\"");
				if (i == guild_ignore_count - 1)
					strcat(buff, "\n");
				else strcat(buff, ",\n");
			}
			strcat(buff, "    ],\n"
						 "    \"commands\": {\n"
			);
			for (int i = 0; i < sizeof(commands) / sizeof(command); ++i) {
				strcat(buff, "        \"");
				strcat(buff, commands[i].command_name);
				strcat(buff, "\": {\n"
							 "            \"executors\": [\n"
				);
				for (int j = 0; j < commands[i].executors_count; ++j) {
					strcat(buff, "                \"");
					strcat(buff, commands[i].executors[j]);
					strcat(buff, "\"");
					if (i == commands[i].executors_count - 1)
						strcat(buff, "\n");
					else strcat(buff, ",\n");
				}
				strcat(buff, "            ],\n"
							 "            \"requiredPermissions\": [\n"
				);

				for (int j = 0; j < commands[i].permissions_count; ++j) {
                	strcat(buff, "                \"");
                	strcat(buff, commands[i].permissions[j]);
                	strcat(buff, "\"");
                	if (i == commands[i].permissions_count - 1)
                		strcat(buff, "\n");
                	else strcat(buff, ",\n");
                }
				strcat(buff, "            ],\n"
                			 "            \"enabled\": "
                );
				strcat(buff, commands[i].enabled ? "true" : "false");
				strcat(buff, "\n        }");
				if (i == sizeof(commands) / sizeof(command) - 1)
					strcat(buff, "\n");
				else strcat(buff, ",\n");
			}
			strcat(buff, "    }\n"
						 "}");

			
			fprintf(fd, "%s", buff);
			free(buff);
			fclose(fd);
		}
		break;
		case 2:
			return 0;
		break;
	}
}
