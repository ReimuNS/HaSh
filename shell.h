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
