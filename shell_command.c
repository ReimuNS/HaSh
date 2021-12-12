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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell_command.h"

ShellCode shell_command_exit(Shell* t_shell, char** t_arguments, size_t t_argumentCount) {
    if (t_argumentCount != 0) {
        fprintf(stderr, "exit: expected 0 arguments but received %ld\n", t_argumentCount);
        return SHELL_INVALID_ARGUMENTS;
    }
    return SHELL_EXIT;
}


ShellCode shell_command_cd(Shell* t_shell, char** t_arguments, size_t t_argumentCount) {
    if (t_argumentCount >= 2) {
        fprintf(stderr, "cd: expected 0 or 1 arguments but received %ld\n", t_argumentCount);
        return SHELL_INVALID_ARGUMENTS;
    }

    const char* argument = NULL;
    if (t_argumentCount == 0) {
        argument = getenv("HOME");
        if (argument == NULL) {
            fprintf(stderr, "cd: the HOME environment variable has not been set\n");
            return SHELL_ERROR;
        }
    }
    else {
        argument = t_arguments[0];
    }
    
    if (shell_set_cwd(t_shell, argument) != 0) {
        fprintf(stderr, "cd: error when trying to change working directory\n"); // TODO: better error message here
        return SHELL_ERROR;
    }
    else {
        return SHELL_OKAY;
    }
}
