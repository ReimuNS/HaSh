#ifndef SHELL_COMMANDS_INCLUDED
#define SHELL_COMMANDS_INCLUDED

#include "shell.h"

typedef ShellCode (*ShellCommand)(Shell*, char**, size_t);

ShellCode shell_command_exit(Shell* t_shell, char** t_arguments, size_t t_argumentCount);
ShellCode shell_command_cd(Shell* t_shell, char** t_arguments, size_t t_argumentCount);

#endif
