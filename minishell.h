#ifndef MINISHELL_H
# define MINISHELL_H

//**INCLUDES
# include "libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>
#include <linux/limits.h>

//**STRUCT
typedef struct s_minishell
{
    char *current_dir; // Current working directory
    char **envp; // Environment variables
    int last_exit_status; // Exit status of the last command
}               t_minishell;

#endif