#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void syncVaultChanges(const char* path) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char addCommand[256] = {0};
	char pushCommand[256] = {0};
	char commitCommand[256] = {0};

	sprintf(addCommand, "git -C %s add .", path);
	sprintf(pushCommand, "git -C %s push", path);
	sprintf(
		commitCommand,
		"git -C %s commit -m \"Automatic sync at %d-%02d-%02d %02d:%02d:%02d\"",
		path,
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec
	);

	system(addCommand);
	system(commitCommand);
	system(pushCommand);
}

int main(void) {
	const char* OBSIDIAN_VAULT_PATH = getenv("OBSIDIAN_VAULT_PATH");

	if (OBSIDIAN_VAULT_PATH == NULL) {
		return 1;
	}

	FILE *commandOutput;
	char commandOutputString[512] = {0};
	char listFilesCommand[256] = {0};
	
	sprintf(listFilesCommand, "git -C %s ls-files --exclude-standard --others --modified", OBSIDIAN_VAULT_PATH);

	commandOutput = popen(listFilesCommand, "r");
	fgets(commandOutputString, sizeof(commandOutputString), commandOutput);

	if (commandOutputString[0] != '\0') {
		syncVaultChanges(OBSIDIAN_VAULT_PATH);
	} 

	return 0;
}
