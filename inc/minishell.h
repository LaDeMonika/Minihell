/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:21:51 by msimic            #+#    #+#             */
/*   Updated: 2024/04/09 11:50:22 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//***************************INCLUDES
# include "../libft/libft.h"
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

//***************************STRUCT
typedef struct s_minishell
{
    char    *user;
    char    *usr_input;
    char    *prompt;
    int     len_prompt;
    pid_t   pid;
    int     exit_status;
    char    **path_array; // Array of paths
    char    *home_dir;
    char    *current_dir; // Current working directory
    char    *command_path; // Path to the command
    char    **envp; // Environment variables
    char    **input_array; // Array of input arguments
    char    **command_history; // Array of command history
    int     history_index; // Index of the command history
    int     last_exit_status; // Exit status of the last command
    int     fd_hostname; // File descriptor for hostname
    int     fd; // File descriptor
}               t_minishell;

//***************************ENUM
/* enum e_return
{
    RET_SUCCESS,
    RET_FAILURE
}; */

enum e_error
{
    ERR_MALLOC,
    ERR_TOO_MANY_ARGS,
    ERR_TOO_FEW_ARGS,
    ERR_INVALID_ARG,
    ERR_PATH_NOT_FOUND
};

//***************************PROTOTYPES
// main
void	init_shell_struct(t_minishell *shell, char **envp);
// builtins
int     ft_echo(t_minishell *shell);
int     ft_is_builtin(t_minishell *shell);
// err
void    ft_error_msg(char err);

#endif