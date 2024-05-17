#ifndef FT_ECHO_H
# define FT_ECHO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>

typedef struct s_minishell
{
    char    **input_array;
    char    **cmd_args;
    int     exit_status;
}   t_minishell;


#endif