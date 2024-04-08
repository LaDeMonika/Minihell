/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:21:51 by msimic            #+#    #+#             */
/*   Updated: 2024/04/08 11:21:45 by msimic           ###   ########.fr       */
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
    char    *usr_input; // User input
    char    *prompt; // Prompt
    int     len_prompt; // Length of the prompt
    char    **path_array; // Array of paths
    char    *home_dir; // Home directory
    char    *current_dir; // Current working directory
    char    *command_path; // Path to the command
    char    **envp; // Environment variables
    char    **input_array; // Array of input arguments
    char    **command_history; // Array of command history
    int     history_index; // Index of the command history
    int     last_exit_status; // Exit status of the last command
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
// error_msg.c
void    ft_error_msg(char err);

#endif