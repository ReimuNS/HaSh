/*
    HaSh is a simple shell program based on bash
    Copyright (C) 2021  ReimuNS

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or 
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <limits.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "shell.h"

char* generate_prompt(const char* t_cwd);
const char* find_last_of(const char* t_string, char t_char);

void intHandler(int t_signal);

static volatile sig_atomic_t at_readline = 0;
sigjmp_buf sigintBuf;

int main(int argc, char* argv[]) {
    Shell* shell = shell_create(NULL);

    signal(SIGINT, intHandler);

    ShellCode code = SHELL_OKAY;
    char* prompt = generate_prompt(shell->cwd);
    char* line = NULL;

    while (
        (
         (at_readline = 1),
         (sigsetjmp(sigintBuf, 1)),
         (line = readline(prompt))
        ) &&
        (code != SHELL_EXIT)
    ) {
        at_readline = 0;

        if (*line != '\0') {
            add_history(line);
            code = shell_execute_command(shell, line);

            free(prompt);
            prompt = generate_prompt(shell->cwd);
        }

        free(line);
        line = NULL;
    }
    free(prompt);

    // Print exit line if exitted using CTRL-D
    if (line == NULL) {
        printf("exit\n");
    }

    shell_destroy(shell);
    return 0;
}

char* generate_prompt(const char* t_cwd) {
    if (t_cwd == NULL) {
        return NULL;
    }

    const char* prefix = "[";
    const char* suffix = "]$ ";
    const char* usernamePCNameSeparator = "@";
    const char* pcNameDirSeparator = " ";

    const char* username = getenv("USER");
    if (username == NULL) {
        username = "unknown";
    }

    char pcName[HOST_NAME_MAX + 1] = {0};
    if (gethostname(pcName, HOST_NAME_MAX + 1) == -1) {
        pcName[0] = '\0';
    }

    const char* currentDir = find_last_of(t_cwd, '/');
    if (currentDir == NULL) {
        currentDir = "unknown";
    }
    else {
        currentDir++;
    }

    const size_t promptLength = 
        strlen(prefix) +
        strlen(username) +
        strlen(usernamePCNameSeparator) +
        strlen(pcName) +
        strlen(pcNameDirSeparator) +
        strlen(currentDir) +
        strlen(suffix);
    char* prompt = malloc(sizeof(char) * (promptLength + 1));

    snprintf(prompt, promptLength + 1, "%s%s%s%s%s%s%s\n", prefix, username, usernamePCNameSeparator, pcName, pcNameDirSeparator, currentDir, suffix);

    return prompt;
}

const char* find_last_of(const char* t_string, char t_char) {
    if (t_string == NULL) {
        return NULL;
    }

    const char* lastSoFar = NULL;
    for (const char* current = t_string; *current != '\0'; current++) {
        if (*current == t_char) {
            lastSoFar = current;
        }
    }
    return lastSoFar;
}

void intHandler(int t_signal) {
    if (t_signal == SIGINT && at_readline) {
        write(STDOUT_FILENO, "\n", 1);
        siglongjmp(sigintBuf, 1);
    }
}
