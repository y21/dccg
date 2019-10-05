#include <stdio.h>

// Constants
#define VERSION "1.2"
#define MAX_TOKEN_LEN 64
#define MAX_PREFIX_LEN 8
#define MAX_PRESENCE_NAME_LEN 32
#define MAX_SNOWFLAKE_LEN 20
#define MAX_IGNORED_SERVERS MAX_PREFIX_LEN
#define MAX_COMMAND_NAME_LEN 24
#define MAX_EXECUTORS MAX_PREFIX_LEN
#define MAX_PERMISSIONS MAX_PREFIX_LEN
#define MAX_PERMISSION_LEN MAX_COMMAND_NAME_LEN

// Structs
typedef struct {
	int type;
	char name[MAX_PRESENCE_NAME_LEN];
} presence;

typedef struct {
	char id[MAX_SNOWFLAKE_LEN];
	char verification_channel[MAX_SNOWFLAKE_LEN];
	char verification_role[MAX_SNOWFLAKE_LEN];
} guild;

typedef struct {
	char command_name[MAX_COMMAND_NAME_LEN];
	char executors[MAX_EXECUTORS][MAX_SNOWFLAKE_LEN];
	char required_permissions[MAX_PERMISSIONS][MAX_PERMISSION_LEN];
} command;

// Data
char token[MAX_TOKEN_LEN];
char prefix[MAX_PREFIX_LEN];
int delete_messages;
presence prs;
int ignored_servers[MAX_IGNORED_SERVERS];

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

			printf("Enter Bot Token: ");
			fgets(token, sizeof(token), stdin);

			printf("ok your token is %s\n", token);

	printf("Enter Prefix: ");
	fgets(prefix, sizeof(prefix), stdin);

	printf("ok your prefix is %s\n", prefix);

		}
		break;
		case 2:
			return 0;
		break;
	}
}
