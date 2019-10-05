#include <stdio.h>

// Constants
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
	
}
