#ifndef SHELL_INCLUDED
#define SHELL_INCLUDED

typedef struct {
    char* cwd;
} Shell;

typedef enum {
    SHELL_OKAY = 0,
    SHELL_ERROR,
    SHELL_PARSE_ERROR,
    SHELL_COMMAND_NOT_FOUND,
    SHELL_INVALID_ARGUMENTS,
    SHELL_EXIT
} ShellCode;

Shell* shell_create();
void shell_destroy(Shell* t_shell);

int shell_set_cwd(Shell* t_shell, const char* t_pwd);

ShellCode shell_execute_command(Shell* t_shell, const char* t_command);

#endif
