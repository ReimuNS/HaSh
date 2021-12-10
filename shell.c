#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"
#include "shell_command.h"

ShellCode shell_execute_built_in_command(Shell* t_shell, const char* t_command, char** t_arguments, size_t t_argumentCount);
ShellCode shell_execute_external_command(Shell* t_shell, const char* t_command, char** t_arguments);

Shell* shell_create() {
    Shell* result = malloc(sizeof(Shell));
    result->cwd = getcwd(NULL, 0);

    return result;
}

void shell_destroy(Shell* t_shell) {
    if (t_shell == NULL) {
        return;
    }

    free(t_shell->cwd);
    free(t_shell);
}

int shell_set_cwd(Shell* t_shell, const char* t_dir) {
    if (t_shell == NULL) {
        return 1;
    }

    const int returnCode = chdir(t_dir);
    if (returnCode == 0) {
        free(t_shell->cwd);
        t_shell->cwd = getcwd(NULL, 0);
    }
    return returnCode;
}

ShellCode shell_execute_command(Shell* t_shell, const char* t_command) {
    char* command = malloc(sizeof(char) * strlen(t_command) + 1);
    strcpy(command, t_command);

    char* program = strtok(command, " \t");
    if (program == NULL) {
        free(command);
        return SHELL_OKAY;
    }

    size_t argumentCount = 0;
    while (strtok(NULL, " \t") != NULL) {
        argumentCount++;
    }

    char** arguments = malloc(sizeof(char*) * (argumentCount + 2));
    arguments[0] = program;
    arguments[argumentCount + 1] = NULL;

    // Copy tokens to token array
    char* currentToken = program + strlen(program) + 1;
    for (size_t i = 0; i < argumentCount; i++) {
        arguments[i + 1] = currentToken;
        currentToken += strlen(currentToken) + 1;
    }

    ShellCode result = shell_execute_built_in_command(t_shell, program, arguments + 1, argumentCount);
    if (result == SHELL_COMMAND_NOT_FOUND) {
        result = shell_execute_external_command(t_shell, program, arguments);
    }

    free(arguments);
    free(command);
    return result;
}

ShellCode shell_execute_built_in_command(Shell* t_shell, const char* t_program, char** t_arguments, size_t t_argumentCount) {
    typedef struct {
        const char* commandName;
        ShellCommand command;
    } CommandPair;

    const CommandPair COMMAND_PAIRS[] = {
        {"cd", shell_command_cd},
        {"exit", shell_command_exit}
    };
    const size_t COMMAND_COUNT = sizeof(COMMAND_PAIRS) / sizeof(CommandPair);

    for (size_t i = 0; i < COMMAND_COUNT; i++) {
        if (strcmp(t_program, COMMAND_PAIRS[i].commandName) == 0) {
            return COMMAND_PAIRS[i].command(t_shell, t_arguments, t_argumentCount);
        }
    }

    return SHELL_COMMAND_NOT_FOUND;
}

ShellCode shell_execute_external_command(Shell* t_shell, const char* t_program, char** t_arguments) {
    ShellCode returnCode = SHELL_OKAY;
    const pid_t childPID = fork();
    switch (childPID) {
        case -1:
            fprintf(stderr, "Failed to create child process\n");
            returnCode = SHELL_ERROR;
        case 0: // we are the child
            execvp(t_program, t_arguments);
            fprintf(stderr, "Error when trying to exec '%s'\n", t_program); // TODO: better error message here
            exit(1);
        default: // we are the parent
            waitpid(childPID, NULL , 0);
            returnCode = SHELL_OKAY;
    }
    return returnCode;
}

