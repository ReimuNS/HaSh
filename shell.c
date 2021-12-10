#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"
#include "shell_command.h"

ShellCode shell_execute_built_in_command(Shell* t_shell, const char* t_command, char** t_arguments, size_t t_argumentCount);
ShellCode shell_execute_external_command(Shell* t_shell, const char* t_command, char** t_arguments);

typedef enum {
    TOKENIZE_OKAY = 0,
    TOKENIZE_ERROR_NULL_STRING,
    TOKENIZE_ERROR_UNMATCHED_QUOTES
} TokenizeCode;

typedef struct {
    TokenizeCode code;
    size_t count;
} TokenizeResult;

TokenizeResult tokenize_result_create(TokenizeCode t_code, size_t t_count);

TokenizeResult shell_tokenize_input(char* t_command);

void remove_first_char(char* t_string);

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

    const TokenizeResult tokenizeResult = shell_tokenize_input(command);
    if (tokenizeResult.code != TOKENIZE_OKAY) {
        fprintf(stderr, "HaSh: error when parsing command '%s'\n", t_command); // TODO: have a better error message here

        free(command);
        return SHELL_PARSE_ERROR;
    }
    const size_t tokenCount = tokenizeResult.count;

    char** arguments = malloc(sizeof(char*) * (tokenCount + 1));
    arguments[tokenCount] = NULL;

    // Copy tokens to token array
    char* currentToken = command;
    for (size_t i = 0; i < tokenCount; i++) {
        arguments[i] = currentToken;
        currentToken += strlen(currentToken) + 1;
    }

    char* program = arguments[0];
    ShellCode result = shell_execute_built_in_command(t_shell, program, arguments + 1, tokenCount - 1);
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
            fprintf(stderr, "HaSh: failed to create child process\n");
            returnCode = SHELL_ERROR;
        case 0: // we are the child
            execvp(t_program, t_arguments);
            fprintf(stderr, "HaSh: error when trying to execute '%s'\n", t_program); // TODO: better error message here
            exit(1);
        default: // we are the parent
            waitpid(childPID, NULL , 0);
            returnCode = SHELL_OKAY;
    }
    return returnCode;
}

TokenizeResult shell_tokenize_input(char* t_command) {
    typedef enum {
        STATE_DEFAULT,
        STATE_DQ_STRING,
        STATE_SQ_STRING,
    } ParseState;

    if (t_command == NULL) {
        return tokenize_result_create(TOKENIZE_ERROR_NULL_STRING, 0);
    }

    ParseState state = STATE_DEFAULT;
    char* p = t_command;
    size_t tokenCount = 0;
    while (*p != '\0') {
        switch (state) {
            case STATE_DEFAULT:
                if (*p == ' ' || *p == '\t') {
                    *p = '\0';
                    p++;
                    tokenCount++;
                }
                else if (*p == '"') {
                    remove_first_char(p);
                    state = STATE_DQ_STRING;
                }
                else if (*p == '\'') {
                    remove_first_char(p);
                    state = STATE_SQ_STRING;
                }
                else {
                    p++;
                }
                break;
            case STATE_DQ_STRING:
                if (*p == '\\') {
                    remove_first_char(p);
                    p++;
                }
                else if (*p == '"') {
                    remove_first_char(p);
                    state = STATE_DEFAULT;
                }
                else {
                    p++;
                }
                break;
            case STATE_SQ_STRING:
                if (*p == '\\') {
                    remove_first_char(p);
                    p++;
                }
                else if (*p == '\'') {
                    remove_first_char(p);
                    state = STATE_DEFAULT;
                }
                else {
                    p++;
                }
                break;
        }
    }

    switch (state) {
        case STATE_SQ_STRING:
        case STATE_DQ_STRING:
            return tokenize_result_create(TOKENIZE_ERROR_UNMATCHED_QUOTES, tokenCount);
        default:
            return tokenize_result_create(TOKENIZE_OKAY, tokenCount + 1);
    }
}

void remove_first_char(char* t_string) {
    if (t_string == NULL) {
        return;
    }

    for (size_t i = 0; t_string[i] != '\0'; i++) {
        t_string[i] = t_string[i + 1];
    }
}

TokenizeResult tokenize_result_create(TokenizeCode t_code, size_t t_count) {
    TokenizeResult result;
    result.code = t_code;
    result.count = t_count;
    return result;
}
