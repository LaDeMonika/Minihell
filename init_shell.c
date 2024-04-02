/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:26:33 by msimic            #+#    #+#             */
/*   Updated: 2024/04/02 15:16:49 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO: Implement init_shell function
//TODO: init shell structure
//TODO: 

void	init_shell_struct(t_minishell *shell, char **envp)
{
    shell->usr_input = NULL;
    shell->prompt = NULL;
    shell->home_dir = getenv("HOME");
    shell->current_dir = NULL;
    shell->command_path = NULL;
    shell->envp = envp;
    shell->input_array = NULL;
    shell->command_history = NULL;
    shell->history_index = 0;
    shell->last_exit_status = 0;
    shell->fd = 0;
}