#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "shell.h"

char* read_line(int fd);
void intHandler(int t_signal);

int main(int argc, char* argv[]) {
    Shell* shell = shell_create(NULL);

    signal(SIGINT, intHandler);

    char* line = NULL;
    while (
        (printf("[%s]$ ", (shell->cwd ? shell->cwd : ""))),
        (fflush(stdout)),
        (line = read_line(STDIN_FILENO))
    ) {
        const ShellCode code = shell_execute_command(shell, line);

        free(line);
        line = NULL;

        if (code == SHELL_EXIT) {
            break;
        }
    }

    // Print exit line if exitted using CTRL-D
    if (line == NULL) {
        printf("exit\n");
    }

    shell_destroy(shell);
    return 0;
}

#define INPUT_BUFFER_SIZE 256
char* read_line(int fd) {
    char inputBuffer[INPUT_BUFFER_SIZE] = {0};
    char* result = NULL;
    ssize_t resultLength = 0;

    ssize_t lineLength = 0;
    while ((lineLength = read(STDIN_FILENO, inputBuffer, INPUT_BUFFER_SIZE))) {
        // Remove trailing new-line
        if (inputBuffer[lineLength - 1] == '\n') {
            inputBuffer[lineLength - 1] = '\0';
            lineLength -= 1;
        }

        // Allocate and copy to new string
        size_t newResultLength = resultLength + lineLength;
        char* newResult = malloc(newResultLength + 1);
        newResult[newResultLength] = '\0';
        if (result != NULL) {
            strncpy(newResult, result, resultLength);
        }
        strncpy(newResult + resultLength, inputBuffer, lineLength);

        // Deallocate old string and assign over old string
        free(result);
        result = newResult;
        resultLength = newResultLength;

        // Stop if a newline was found
        if (lineLength != INPUT_BUFFER_SIZE) {
            break;
        }
    }

    return result;
}

void intHandler(int t_signal) {
    ;
}
