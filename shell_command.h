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

#ifndef SHELL_COMMANDS_INCLUDED
#define SHELL_COMMANDS_INCLUDED

#include "shell.h"

typedef ShellCode (*ShellCommand)(Shell*, char**, size_t);

ShellCode shell_command_exit(Shell* t_shell, char** t_arguments, size_t t_argumentCount);
ShellCode shell_command_cd(Shell* t_shell, char** t_arguments, size_t t_argumentCount);

#endif
