/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:26:33 by msimic            #+#    #+#             */
/*   Updated: 2024/05/16 22:19:36 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdio.h>
#include <unistd.h>

void	init_shell_struct(t_minishell *shell, char **envp)
{

	shell->envp = envp;
	shell->prompt = NULL;
	shell->usr_input = NULL;
	shell->line_count = 0;
	shell->input_array = NULL;
	shell->pipes_total = 0;
	shell->list = NULL;
	shell->pid = NULL;
	shell->input_file = NULL;
	shell->command_array = NULL;
	shell->path_array = NULL;
}
void	init_input_iteration(t_minishell *shell)
{
	free_and_reset_ptr((void **)&shell->prompt);
	free_and_reset_ptr((void **)&shell->usr_input);
	free_and_reset_array((void ***)&shell->input_array);
	free_and_reset_array((void ***)&shell->list);
	free_and_reset_ptr((void **)&shell->input_file);
	free_and_reset_array((void ***)&shell->path_array);
	free_and_reset_array((void ***)&shell->command_array);

	shell->line_count++;
	shell->pipes_total = 0;
	shell->pid = NULL;
	shell->input_file = NULL;
	shell->unexpected_token = NULL;
}

